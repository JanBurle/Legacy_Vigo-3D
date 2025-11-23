// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Asynchronicity support.
 *  Threads, mutexes, critical sections, semaphores.
 */

#ifndef VIGO_COMMON_ASYNC_HPP_INCLUDED
#define VIGO_COMMON_ASYNC_HPP_INCLUDED

  #include "./types.hpp"

NAMESPACE_VIGO(util) //========================================================
/** \defgroup grp_async_hpp Asynchronicity
@{*/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  #ifdef WIN32
    #define __FASTCALL __fastcall
  #else
    #define __FASTCALL
  #endif
#endif

  /** \name Atomic operations
  @{*/
  /// Increments \c target and returns the new value.
  int __FASTCALL atomic_increment(int& target);
  /// Decrements \c target and returns the new value.
  int __FASTCALL atomic_decrement(int& target);
  /// Sets \c target to a \c value and returns the old value.
  int __FASTCALL atomic_exchange (int& target,int value);
  /**@}*/

//----------------------------------------------------------------------------

  /// Mutex
  class mutex
  {
    NO_GEN(mutex)

  protected:
    pvoid m_mtx;

  public:
    mutex();
   ~mutex();
    /// Locks (acquires) the mutex.
    /** If another thread has the mutex locked, the call blocks. The mutex is
     *  reentrant, i.e. if a thread has successfully locked a mutex, repeated
     *  calls to \c lock() will not block. To unlock a mutex, the thread must
     *  call \c unlock() the same number of times.
     */
    void lock();
    /// Unlocks (releases) the mutex.
    void unlock();
  };

//----------------------------------------------------------------------------

  /// Semaphore
  class semaphore
  {
    NO_GEN(semaphore)

  protected:
    pvoid m_sem;

  public:
    /// Constructs a semaphore.
    /** @param initialCount The initial value of available lanes. */
    semaphore(uint initialCount=0);
   ~semaphore();
    /// Waits for an available lane.
    void wait();
    /// Free a lane.
    void post();
  };

//----------------------------------------------------------------------------

  /// Critical section; automatically locks and unlocks a mutex.
  /** Used to ensure an exclusive execution of a code region by a thread,
   *  safely in an exception-enabled environment, as in:
   *  \code
   *  mutex mx;
   *
   *  {	                         // start a statement block
   *    critical_section cs(mx); // automatically locks the mutex
   *    ... code ...		 // protected code region
   *  }				 // cs is destructed, unlocks the mutex
   *  \endcode
   */
  class critical_section
  {
    NO_GEN(critical_section)

  protected:
    mutex &m_mutex;

  public:
    critical_section(mutex&);
   ~critical_section();
  };

//----------------------------------------------------------------------------
  /// Thread
  /** To create a thread of execution, declare a subclass and override the
   *  method \c body(). An example of using threads:
   *  \code
   *  class MyThread: public vigo::util::thread
   *  {
   *    void body();
   *  };
   *
   *  void MyThread::body()
   *  {
   *    while(!signaled())
   *    {
   *      DoSomething(); // an activity, approx. every second
   *      sleep(1000);
   *    }
   *  }
   *
   *  void main()
   *  {
   *    MyThread t1, t2, t3;                   // create several threads
   *    t1.start(); t2.start(); t3.start();    // start the threads
   *    DoSomethingMeanwhile();                // an independent activity
   *    t1.signal(); t2.signal(); t3.signal(); // notify the threads
   *    t1.wait(); t2.wait(); t3.wait();       // wait for them to exit
   *  }                                        // the threads are destroyed
   *  \endcode
   */
  class thread
  {
    NO_GEN(thread)

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS START_FAILED,CANT_WAIT,SET_FAILED,KEYCREATE_FAILED
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

  public:
    /// thread local storage
    class tls
    {
      NO_GEN(tls)
    private:
    #ifdef WIN32
      ulong m_key;
    #else
      pvoid m_key;
    #endif

    public:
      /// Allocates a thread local storage.
      tls() throw(exc);
      /// Deletes the local storage.
     ~tls();

      /// Stores a value, typically a pointer to a memory block
      /// allocated by the thread.
      void  set(pcvoid) throw(exc);
      /// Retrieves the value.
      pvoid get() const;
    };

  public:
    /// A thread handle type.
    typedef pvoid handle;

  protected:
  #ifdef WIN32
    uint    m_id;
  #endif
    handle  m_handle;

    typedef int bint;   ///< \c int used as \c bool
    bint    m_started;
    bint    m_signaled;
    bint    m_finished;
    bool    m_selfDelete;

  public:
    /** A thread object that has the selDelete attribute set when the body() exits
        will delete itself by calling "delete this". Proceed with caution.
     */
    thread(bool selfDelete=false);
    virtual ~thread();

    GETSET(SelfDelete,SetSelfDelete,bool,m_selfDelete)

  private:
  #ifdef WIN32
    static uint __stdcall threadproc(pvoid pThread);
  #else
    static pvoid threadproc(pvoid pThread);
  #endif
    virtual void _body();

  protected:
    /// The thread code. You must override this method in your derived class.
    /// When \c body() returns, the thread stops and \c finished() becomes true.
    virtual void body() = 0;

  public:
    /// Start the thread. \c started() becomes true.
    void start() throw(exc);

    /// Has the thread been started? Stays true even after the thread finishes.
    bool started()  const;
    /// Has the thread been signalled?
    bool signaled() const;
    /// Has the thread finished? (i.e. the \c body() has returned)
    bool finished() const;
    /// Is the thread running?
    bool running()  const { return started() && !finished(); }

    /// Signal the thread (request that it terminates). \c signaled() becomes true.
    /** Threads should periodically check \c signaled() and terminate if so
     *  requested.
     */
    virtual void signal();

    /// Wait for the thread to terminate (finish).
    void wait() throw(exc);

    /// Take a nap.
    static void sleep(ulong miliSeconds);

    // Is the code being executed in this object's thread?
    /** An example:
     *  \code
     *  class MyThread: public vigo::util::thread
     *  {
     *    void body();
     *  public:
     *    void Fork();
     *  };
     *
     *  void MyThread::body()
     *  {
     *    while(!signaled())
     *      Fork();          // will execute CallingMyself();
     *  }
     *
     *  void MyThread::Fork()
     *  {
     *    if(isSelf())
     *      CallingMyself();
     *    else
     *      CalledFromOutside();
     *  }
     *
     *  void main()
     *  {
     *    MyThread t; t.start();
     *    t.Fork();         // will execute CalledFromOutside();
     *  }
     *  \endcode
     */
    bool isSelf() const;

    enum ePriority {PRIORITY_NORMAL, PRIORITY_HIGH, PRIORITY_LOW};
    /// Sets the thread priority.
    void setPriority(ePriority);
  };

//----------------------------------------------------------------------------
  /// Thread that is preallocated and can be reused ovear and over again.
  /** Creating and destroying many temporary threads may be expensive. In such
   *  a case one can pre-allocate a pool of threads and re-use them repeatedly.
   *  An example:
   *  \code
   *  class MyThread: public vigo::util::pooled_thread
   *  {
   *    void body();
   *  };
   *
   *  void MyThread::body()
   *  {
   *    SomeShortActivity();
   *  }
   *
   *  void main()
   *  {
   *    MyThread t; t.start();    // create a pooled thread
   *    for(uint i=0; i<12; ++i)  // repeat a dozen times
   *    {
   *      t.enter();              // start SomeShortActivity()
   *      DoSomethingMeanwhile(); // an independent activity
   *      t.wait_leave();         // wait for the thread
   *      DoSomethingElse();
   *    }
   *    t.signal();               // notify the thread
   *    t.wait();                 // wait for it to exit
   *  }
   *  \endcode
   */
  class pooled_thread: public thread
  {
    NO_GEN(pooled_thread)
    SUPER(thread)

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS NOT_RUNNING
    DECLARE_EXC
    #undef EXC_IDS
#endif

    pooled_thread()
      : super(), m_semEntry(), m_semExit()
    {
    }

   ~pooled_thread();

  private:
    semaphore m_semEntry,
              m_semExit;

    virtual void _body();

  public:
    /// Start executing the thread's \c body(), one pass.
    virtual void enter()      throw(exc);
    /// Wait for \c body() to return;
    virtual void wait_leave();
    /// Request the thread to terminate.
    virtual void signal();
  };

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
