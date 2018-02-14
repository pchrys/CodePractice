#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>
#include <cstring>
#include <random>

#include "MemoryManager.h"

const size_t MemoryManager::m_PAGE_SIZE;

MemoryManager::MemoryManager(size_t sz)
{
    int n = sz / m_PAGE_SIZE;
    int r = sz % m_PAGE_SIZE;
    r = r ? 1 : 0;
    int m = (n + r) * m_PAGE_SIZE;

    void* p = malloc(m);
    memset(p, 0x0, m);

    m_addr = reinterpret_cast<unsigned long>(p);
    m_size = m;

    void* q = malloc(sizeof(Node));
    new(q) Node;

    m_pRoot = reinterpret_cast<Node*>(q);
    m_pRoot->addr = m_addr;
    m_pRoot->pages = (n + r);
}

MemoryManager::~MemoryManager()
{
    void* p = reinterpret_cast<void*>(m_pRoot->addr);
    free(p);
    m_pRoot = nullptr;
}
void* MemoryManager::allocate(size_t sz)
{
    size_t sz1 = sz + sizeof(long);

    int n = sz1 / m_PAGE_SIZE;
    int r = sz1 % m_PAGE_SIZE;
    r = r ? 1 : 0;
    int m = (n + r);

    unsigned long addr = 0;

    allocateMemory(m_pRoot, m, addr);

    if (addr != 0)
    {
        void* p = reinterpret_cast<void*>(addr);
        int* q = reinterpret_cast<int*>(p);
        *q = m;
        p = reinterpret_cast<void*>(reinterpret_cast<unsigned long>(p) + sizeof(int));

        return p;
    }

    printf("Memory is exhausted, and cannot allocate required memory inside %s() at %s:%d \n",
           __func__, __FILE__, __LINE__);

    return nullptr;
}

void MemoryManager::allocateMemory(Node* p, size_t pages, unsigned long &addr)
{
    if (!p || addr)
    {
        return;
    }

    if (!p->leaf)
    {
        //printf("pages:%d, %s() is called at %s:%d \n", (int)pages,  __func__, __FILE__, __LINE__);
        allocateMemory(p->left, pages, addr);
        allocateMemory(p->right, pages, addr);
    }
    else
    {
        if (p->used)
        {
            // printf("p->pages:%d, pages:%d, node:%p,  %s() is called at %s:%d \n",
            //        (int)p->pages, (int)pages,  p, __func__, __FILE__, __LINE__);

            return;
        }

        std::lock_guard<std::recursive_mutex> lg(m_mutex);
        // split the node
        if (p->pages == pages)
        {
            addr = p->addr;
            p->used = true;
        }
        else if(p->pages > pages)
        {

            // printf("p->pages:%d, pages:%d, node:%p,  %s() is called at %s:%d \n",
            //        (int)p->pages, (int)pages,  p, __func__, __FILE__, __LINE__);

            addr = p->addr;
            splitNode(p, pages);
        }
    }
}

void MemoryManager::splitNode(Node* p, size_t n)
{
    if (!p || p->pages <= n || p->pages <= 1)
    {
        return;
    }

    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    void* u = malloc(sizeof(Node));
    void* v = malloc(sizeof(Node));

    new (u) Node;
    new (v) Node;
    Node* left = reinterpret_cast<Node*>(u);
    Node* right = reinterpret_cast<Node*>(v);

    // Node* left = new Node();
    // Node* right = new Node();


    left->pages = p->pages / 2;
    right->pages = p->pages - left->pages;

    left->addr = p->addr;
    right->addr = p->addr + left->pages * m_PAGE_SIZE;

    p->leaf = false;
    p->used = true;
    p->left = left;
    p->right = right;
    p->addr = right->addr;

    // printf("pages: %5d, Node:%p is split into two sub nodes, left:%p(addr:%lx), right:%p(addr:%lx), %s() at %s:%d \n",
    //        (int)p->pages, p,  left, left->addr,
    //        right, right->addr, __func__, __FILE__, __LINE__);

    if (n == left->pages)
    {
        left->used = true;
    }
    else if (n < left->pages)
    {
        splitNode(left, n);
    }
    else
    {
        left->used = true;
        splitNode(right, n - left->pages);
    }
}
void MemoryManager::deallocate(void* ptr)
{
    void* p = reinterpret_cast<void*>(reinterpret_cast<unsigned long>(ptr) - sizeof(int));
    int* q = reinterpret_cast<int*>(p);
    size_t pages = *q;

    deallocateMemory(nullptr, m_pRoot, reinterpret_cast<unsigned long>(p), pages);

    // printf("pages: %d,  p: %p,  %s() at %s:%d \n",
    //        (int)pages, p,   __func__, __FILE__, __LINE__);
}

void MemoryManager::deallocateMemory(Node* pp, Node* p, unsigned long addr, size_t pages)
{
    if (!p)
    {
        return;
    }

    if (!p->leaf)
    {
        if (addr < p->addr)
        {
            deallocateMemory(p, p->left, addr, pages);
        }
        else
        {
            deallocateMemory(p, p->right, addr, pages);
        }
    }
    else
    {
        if(p->used && p->addr == addr)
        {
            // find the node we allocated the memory from it
            assert(p->used);
            p->used = false;

            if (pages > p->pages)
            {
                unsigned long addr = p->addr + p->pages * m_PAGE_SIZE;
                if (pp)
                {
                    deallocateMemory(pp, pp->right, addr, pages - p->pages);
                }
                else
                {
                    printf("Memrory management is messed up\n");
                }
            }
        }
    }

    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    if (p->left != nullptr && p->left->used == false && p->right != nullptr && p->right->used == false)
    {
        // remove two child kids;
        p->addr = p->left->addr;
        p->pages = p->left->pages + p->right->pages;
        p->leaf = true;
        p->used = false;

        // printf("two sub nodes are merged into one node, %s() at %s:%d \n",
        //         __func__, __FILE__, __LINE__);

        p->left->~Node();
        p->right->~Node();

        free(p->left);
        free(p->right);
    }
}

#if 1
const int MEMORY_POOL_SIZE = 4096*1024*256;    //1G

MemoryManager gMemoryManager(MEMORY_POOL_SIZE);
void* operator new(size_t size)
{
    return gMemoryManager.allocate(size);
}

void* operator new[](size_t size)
{
    return gMemoryManager.allocate(size);
}

void operator delete(void* ptr)
{
    gMemoryManager.deallocate(ptr);
}

void operator delete[](void* ptr)
{
    gMemoryManager.deallocate(ptr);
}
#endif

#define DEBUG
//#undef DEBUG
int main()
{
    unsigned long size = 4096 * 1024*256;

    MemoryManager memManager(size);

    std::vector<void*> memVec;
    int n = 512;
    for (int i = 0; i < n; i++)
    {
#ifdef DEBUG
        auto p = memManager.allocate(4096);
#else
        auto p = malloc(4096);
#endif
        //printf("memory address : %p \n", p);
        memVec.push_back(p);
    }

    for (int i = 0; i < memVec.size(); i++)
    {
        //printf("memory address : %p \n", memVec[i]);
    }

    for (int i = 0; i < memVec.size(); i++)
    {
        //printf("deallocate memory address : %p \n", memVec[i]);
#ifdef DEBUG
        memManager.deallocate(memVec[i]);
#else
        free(memVec[i]);
#endif
    }

    memVec.clear();

    std::default_random_engine e;
    e.seed(1);
    std::uniform_int_distribution<unsigned> u(1, 32);

    int q = 2000;
    for(int i = 0; i < q; i++)
    {
        unsigned int sz = u(e);
        sz = sz*4096; //4096;
#ifdef DEBUG
        auto p = memManager.allocate(sz);
#else
        auto p = malloc(4096);
#endif

        memVec.push_back(p);

    }
    for (int i = 0; i < memVec.size(); i++)
    {
        //printf("memory address : %p \n", memVec[i]);
    }

    for (int i = 0; i < memVec.size(); i++)
    {
        //printf("deallocate memory address : %p \n", memVec[i]);
#ifdef DEBUG
        memManager.deallocate(memVec[i]);
#else
        free(memVec[i]);
#endif
    }

    return 0;
}
