// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "./async.hpp"

#ifdef WIN32
  #define _WINSOCKAPI_   // prevent inclusion of winsock.h in windows.h
  #include <windows.h>
  #include <process.h>
#else
  #include <pthread.h>
  #ifndef __bsdi__
    #include <semaphore.h>
  #endif
#endif

#ifdef __SUNOS__
  #include <unistd.h>
#endif

NAMESPACE_VIGO(util) //========================================================

#ifdef WIN32

  int __FASTCALL atomic_increment(int& target)
  {
    return ::InterlockedIncrement(&target);
  }

  int __FASTCALL atomic_decrement(int& target)
  {
    return ::InterlockedDecrement(&target);
  }

  int __FASTCALL atomic_exchange (int& target,int value)
  {
    return ::InterlockedExchange(&target,value);
  }

#else
// other platforms: mutex locking

  static mutex& AtomicMutex()
  {
    static mutex atomic_mutex;
    return atomic_mutex;
  }

  int atomic_increment(int& target)
  {
    critical_section cs(AtomicMutex());
    return ++target;
  }

  int atomic_decrement(int& target)
  {
    critical_section cs(AtomicMutex());
    return --target;
  }

  int atomic_exchange(int& target,int value)
  {
    critical_section cs(AtomicMutex());
    int old = target;
    target  = value;
    return old;
  }

#endif

//----------------------------------------------------------------------------
/// Mutex

  mutex::mutex()
    : m_mtx(NULL)
  {
  #ifdef WIN32
    m_mtx = new CRITICAL_SECTION();
    #define M_MTX ((CRITICAL_SECTION*)m_mtx)
    InitializeCriticalSection(M_MTX);
  #else
    m_mtx = new pthread_mutex_t();
    #define M_MTX ((pthread_mutex_t*)m_mtx)
    pthread_mutex_init(M_MTX, 0);
  #endif
  }

  mutex::~mutex()
  {
  #ifdef WIN32
    DeleteCriticalSection(M_MTX);
  #else
    pthread_mutex_destroy(M_MTX);
  #endif
    delete M_MTX;
  }

  void mutex::lock()
  {
  #ifdef WIN32
    EnterCriticalSection(M_MTX);
  #else
    pthread_mutex_lock(M_MTX);
  #endif
  }

  void mutex::unlock()
  {
  #ifdef WIN32
    LeaveCriticalSection(M_MTX);
  #else
    pthread_mutex_unlock(M_MTX);
  #endif
  }

//----------------------------------------------------------------------------
/// Semaphore

  semaphore::semaphore(uint initialCount)
   : m_sem(NULL)
  {
  #ifdef WIN32
    m_sem = CreateSemaphore(NULL,initialCount,LONG_MAX,NULL);
    #define M_SEM ((HANDLE)m_sem)
  #else
    m_sem = new sem_t;
    #define M_SEM ((sem_t*)m_sem)
    if(0!=sem_init(M_SEM,0,initialCount))
      SYSTEM_ERROR("semaphore::semaphore failed")
  #endif
  }

  semaphore::~semaphore()
  {
  #ifdef WIN32
    CloseHandle(M_SEM);
  #else
    sem_destroy(M_SEM);
    delete M_SEM;
  #endif
  }

  void semaphore::wait()
  {
  #ifdef WIN32
    WaitForSingleObject(M_SEM,INFINITE);
  #else
    sem_wait(M_SEM);
  #endif
  }

  void semaphore::post()
  {
  #ifdef WIN32
    ReleaseSemaphore(M_SEM,1,NULL);
  #else
    sem_post(M_SEM);
  #endif
  }

//----------------------------------------------------------------------------

  critical_section::critical_section(mutex& m)
    : m_mutex(m)
  {
    m_mutex.lock();
  }

  critical_section::~critical_section()
  {
    m_mutex.unlock();
  }

//----------------------------------------------------------------------------

  thread::tls::tls() throw(thread::exc)
    : m_key(NULL)
  {
  #ifdef WIN32
    m_key = (ulong)TlsAlloc();
    #define M_KEY ((DWORD)m_key)
  #else
    m_key = new pthread_key_t();
    #define M_KEY ((pthread_key_t*)m_key)
    if(0!=pthread_key_create(M_KEY,NULL))
    {
      delete M_KEY;
      m_key = NULL;
      THROW_EXC2(KEYCREATE_FAILED,"thread::tls::tls() failed")
    }
  #endif
  }

  thread::tls::~tls()
  {
  #ifdef WIN32
    TlsFree(M_KEY);
  #else
    if(m_key)
      pthread_key_delete(*M_KEY);
  #endif
  }

  void thread::tls::set(pcvoid value) throw(exc)
  {
  #ifdef WIN32
    if(!TlsSetValue(M_KEY,(pvoid)value))
  #else
    if(0!=pthread_setspecific(*M_KEY,value))
  #endif
      THROW_EXC2(SET_FAILED,"thread::tls::set() failed")
  }

  pvoid thread::tls::get() const
  {
  #ifdef WIN32
    return TlsGetValue(M_KEY);
  #else
    return pthread_getspecific(*M_KEY);
  #endif
  }

//----------------------------------------------------------------------------

  thread::thread(bool selfDelete):
  #ifdef WIN32
    m_id(0),
  #endif
    m_handle(NULL),
    m_started(false), m_signaled(false), m_finished(false),
    m_selfDelete(selfDelete)
  {
  }

  thread::~thread()
  {
  	if(m_handle && !m_selfDelete)
  	{
      signal(); wait();
  	}
  #ifdef WIN32
    CloseHandle(m_handle);
  #endif
  }

#ifdef WIN32
  uint __stdcall thread::threadproc(pvoid pThread)
#else
  pvoid thread::threadproc(pvoid pThread)
#endif
  {
    assert(NULL!=pThread);
    thread *t = reinterpret_cast<thread*>(pThread);
    t->_body();
    if(t->m_selfDelete)
    {
    #ifdef WIN32
      CloseHandle(t->m_handle);
    #else
      pthread_detach((pthread_t)(t->m_handle));
    #endif
      delete t;
    }
    return 0;
  }

  void thread::_body()
  {
    body();
    m_finished = true;
  }

  void thread::start() throw(thread::exc)
  {
    if(!atomic_exchange(m_started,true))
    {
    #ifdef WIN32
      m_handle = (handle)_beginthreadex(NULL,0,threadproc,this,0,&m_id);
      RUNTIME_CHECK_EXC3(m_handle, START_FAILED, "thread::start() failed")

    #else
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

      if(0 != pthread_create((pthread_t*)&m_handle,&attr,threadproc,this))
        THROW_EXC2(START_FAILED,"thread::start() failed")

      pthread_attr_destroy(&attr);
    #endif
    }
  }

  bool thread::started() const
  {
    return false!=m_started;
  }

  bool thread::signaled() const
  {
    return false!=m_signaled;
  }

  bool thread::finished() const
  {
    return false!=m_finished;
  }

  void thread::signal()
  {
    m_signaled = true;
  }

  void thread::wait() throw(thread::exc)
  {
    if(isSelf())
      THROW_EXC2(CANT_WAIT,"thread::wait() cannot wait on itself")

  #ifdef WIN32
    WaitForSingleObject(m_handle,INFINITE);
    CloseHandle(m_handle);
  #else
    pthread_join  ((pthread_t)m_handle,NULL);
    pthread_detach((pthread_t)m_handle);
  #endif

    m_handle = NULL;
  }

  void thread::sleep(ulong miliSeconds)
  {
  #ifdef WIN32
    Sleep(miliSeconds);
  #else
    usleep(1000*miliSeconds);
  #endif
  }

  bool thread::isSelf() const
  {
  #ifdef WIN32
    return GetCurrentThreadId() == m_id;
  #else
    return 0!= pthread_equal(pthread_self(),(pthread_t)m_handle);
  #endif
  }

  void thread::setPriority(ePriority p)
  {
  #ifdef WIN32
    int pri = THREAD_PRIORITY_NORMAL;
    switch(p)
    {
    case PRIORITY_HIGH:
      pri = THREAD_PRIORITY_ABOVE_NORMAL;
      break;

    case PRIORITY_LOW:
      pri = THREAD_PRIORITY_BELOW_NORMAL;
      break;

    default:
      break;
    }

    SetThreadPriority(m_handle,pri);

  #else
    struct sched_param sp;
    int policy;

    #ifndef PTHREAD_DEFAULT_PRIORITY // OpenBSD is missing those
    #define PTHREAD_DEFAULT_PRIORITY 15
    #endif

    #ifndef PTHREAD_MIN_PRIORITY
    #define PTHREAD_MIN_PRIORITY     0
    #endif

    #ifndef PTHREAD_MAX_PRIORITY
    #define PTHREAD_MAX_PRIORITY     31
    #endif

    assert(PTHREAD_MIN_PRIORITY     < PTHREAD_DEFAULT_PRIORITY);
    assert(PTHREAD_DEFAULT_PRIORITY < PTHREAD_MAX_PRIORITY);

    if(0==pthread_getschedparam(pthread_self(), &policy, &sp))
    {
      int pri = PTHREAD_DEFAULT_PRIORITY;
      switch(p)
      {
      case PRIORITY_HIGH:
        pri = PTHREAD_MAX_PRIORITY;
        break;

      case PRIORITY_LOW:
        pri = PTHREAD_MIN_PRIORITY;
        break;

      default:
        break;
      }

      pthread_setschedparam(pthread_self(), policy, &sp);
    }
  #endif
  }

//----------------------------------------------------------------------------

  pooled_thread::~pooled_thread()
  {
  }

  void pooled_thread::_body()
  {
    while(!signaled())
    {
      m_semEntry.wait();   // wait for green

      if(!signaled())
        body();

      m_semExit.post();  // signal
    }

    m_finished = true;
  }

  void pooled_thread::enter() throw(pooled_thread::exc)
  {
    RUNTIME_CHECK_EXC3(running(), NOT_RUNNING,
                       "thread::enter(): the thread is not running")
    m_semEntry.post();
  }

  void pooled_thread::wait_leave()
  {
    m_semExit.wait();
  }

  void pooled_thread::signal()
  {
    super::signal();
    enter(); // release if waiting
  }

NAMESPACE_END() // eof

