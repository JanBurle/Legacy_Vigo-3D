// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief splay trees
 *
 */

#ifndef VIGO_COMMON_SPLAYTREE_HPP_INCLUDED
#define VIGO_COMMON_SPLAYTREE_HPP_INCLUDED

  #include "types.hpp"

NAMESPACE_VIGO(util) //========================================================

#define IDX_CONST_REF__(elem) ((Idx const&)(elem))

  /// Elem must be a subclass of Idx
  template<typename Elem, typename Idx>
  class splaytree
  {
    NO_GEN(splaytree)

  protected:
    struct node;

    struct node0 // a node
    {
      node0(): left(NULL), right(NULL) { }
      node *left, *right; // branches, NULL for leaves
    };

    struct node: node0 // a node with a value
    {
      SUPER(node0)
      node(Elem const& e): super(), elem(e) { }
      Elem elem;
    };

  public:
    virtual int Compare(Idx const&, Idx const&) const = 0;

  private:
    mutable node *m_root;     // the tree root

  public:
    splaytree()
      : m_root(NULL)
    {
    #ifdef _DEBUG
      // Elem must be a subclass of Idx - check
      //>>>??Elem *pElem = NULL;
      //>>> ?? Idx  *pIdx  = static_cast<Idx*>(pElem); pIdx = NULL;
    #endif
    }

    virtual ~splaytree()
    {
      erase();
    }

    GETTER(empty, bool, NULL==m_root)

  // operations
  public:
    /// Inserts a copy of \c elem. Duplicates fail.
    bool insert(Elem const& elem)
    {
      if(NULL==m_root)    // first item
      {
        m_root = new node(elem); return true;
      }

      // not the first item to insert
      m_root = splay(m_root,elem);   // find it

      int cmp = Compare(IDX_CONST_REF__(elem),IDX_CONST_REF__(m_root->elem));
      if(0==cmp) // duplicate key
        return false;

      node *newNode = new node(elem);

      if(0 > cmp)                    // elem < m_root->elem
      {
        // insert left
        newNode->left   = m_root->left;
        newNode->right  = m_root;
        m_root ->left   = NULL;

        m_root = newNode;
        return true;
      }

      // 0 < cmp                    // elem > m_root->elem
      // insert right
      newNode->right  = m_root->right;
      newNode->left   = m_root;
      m_root->right   = NULL;

      m_root = newNode;
      return true;
    }

    bool remove(Idx const& idx)
    {
      if(NULL==m_root)    // empty tree
        return false;

      m_root = splay(m_root,idx);        // find it
      if(0!=Compare(idx,IDX_CONST_REF__(m_root->elem))) // not found
        return false;

      node *nodeToRemove = m_root;

      if(NULL==m_root->left)
        m_root = m_root->right;
      else
      {
        node *newRoot = splay(m_root->left,idx);
        // item is greater than anything in the left subtree,  newRoot->right is now NULL
        assert(NULL==newRoot->right);
        newRoot->right = m_root->right;
        m_root = newRoot;
      }

      delete nodeToRemove;
      return true;
    }

    Elem const* find(Idx const& idx) const
    {
      if(NULL==m_root) return NULL;

      m_root = splay(m_root,idx);
      if(0==Compare(idx,IDX_CONST_REF__(m_root->elem)))
        return &(m_root->elem); // found

      return NULL;
    }

    Elem const* findGE(Idx const& idx) const
    {
      if(NULL==m_root) return NULL;

      m_root = splay(m_root,idx);
      if(0>=Compare(idx,IDX_CONST_REF__(m_root->elem)))
        return &(m_root->elem);     // found an equal or a greater elem
      // Compare() > 0, look for a greater item
      return m_root->right ? &(m_root->right->elem) : NULL;
      // if NULL, there was no greater elem
    }

    Elem const* min() const
    {
      node const *n = min(m_root);
      if(!n) return NULL;
      m_root = splay(m_root,IDX_CONST_REF__(n->elem));
      return &(m_root->elem);
    }

    Elem const* max() const
    {
      node const *n = max(m_root);
      return n ? &(n->elem) : NULL;
    }

    Elem const* prev() const
    {
      // find the maximum item in the left tree
      node *n;
      if(NULL==(n=m_root->left)) return NULL;
      m_root = splay(m_root,IDX_CONST_REF__(max(n)->elem));
      return &(m_root->elemi);
    }

    Elem const* next() const
    {
      // find the minimum item in the right tree
      node *n;
      if(NULL==m_root || NULL==(n=m_root->right)) return NULL;
      m_root = splay(m_root,IDX_CONST_REF__(min(n)->elem));
      return &(m_root->elem);
    }

    Elem const* current() const
    {
      if(NULL==m_root) return NULL;
      return &(m_root->elem);
    }

    void erase()
    {
      while(NULL!=m_root)
      {
        // make the leftmost node the root
        node *left;
        if(NULL!=(left=m_root->left))
        {
          m_root->left = left->right;
          left->right  = m_root;
          m_root = left;
          continue;
        }

        // there is no lefter node: NULL==left
        node *nodeToDelete = m_root;
        m_root = m_root->right;
        delete nodeToDelete;
      }
    }

  private:
    /** Given a root node and an element, finds the element and makes the node the new root.
     *  If the item is not found, either one of the two closest (lesser than and greater than) nodes
     *  becomes the new root.
     */
    node* splay(node* root,Idx const& idx) const
    {
      if(NULL==root) return NULL;

      node *t = root;
      node0 N;       // N.m_left = N.m_right = NULL
      node *l=(node*)&N, *r=(node*)&N;

      for(;;)
      {
        int cmp = Compare(idx,IDX_CONST_REF__(t->elem));
        if(0 > cmp)                       // elem < t->elem
        {
          node *tl = t->left;
          if(NULL==tl) break;             // no more left descend

          if(0 > Compare(idx,IDX_CONST_REF__(tl->elem)))
          {
            t->left   = tl->right;        // rotate right
            tl->right = t;
            t         = tl;
            if(NULL==t->left) break;
          }

          r->left = t;                    // link right
          r = t;
          t = t->left;
        }
        else
        if(0 < cmp)                       // elem > t->m_elem
        {
          node *tr = t->right;
          if(NULL==tr) break;             // no more right descend

          if(0 < Compare(idx,IDX_CONST_REF__(tr->elem)))
          {
            t->right  = tr->left;         // rotate left
            tr->left  = t;
            t         = tr;
            if(NULL==t->right) break;
          }

          l->right = t;                   // link right
          l = t;
          t = t->right;
        }
        else                              // equal
        {
          break;
        }
      }

      l->right  = t->left;
      r->left   = t->right;
      t->left   = N.right;
      t->right  = N.left;

      return t;
    }

    node const* min(node* root) const
    {
      if(NULL==root) return NULL;

      // find the leftmost node n
      node *n,*t;
      for(n = root; NULL!=(t=n->left); n = t)
        ;
      return n;
    }

    node const* max(node* root) const
    {
      if(NULL==root) return NULL;

      // find the rightmost node n
      node *n,*t;
      for(n = root; NULL!=(t=n->m_right); n = t)
        ;
      return n;
    }
  };

  template<typename Elem>
  class splaytree_simple: public splaytree<Elem,Elem>
  {
    typedef splaytree<Elem,Elem> super;

    splaytree_simple(splaytree_simple const&);
    splaytree_simple& operator=(splaytree_simple const&);

  protected:
    int Compare(Elem const& e1, Elem const& e2) const
    {
      if(e1<e2) return -1;
      if(e1>e2) return +1;
      return 0;
    }

  public:
    splaytree_simple(): super()
    {
    }
  };

#undef IDX_CONST_REF__

NAMESPACE_END() //=============================================================

#endif //...INCLUDED

// eof
