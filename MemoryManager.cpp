#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>
#include <cstring>
#include <random>

#include <vector>
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

    void* q = malloc(sizeof(Node));
    new(q) Node;  //placement new, create Node object

    m_pRoot = reinterpret_cast<Node*>(q);
    m_pRoot->addr = m_addr;
    m_pRoot->pages = (n + r);

    m_unusedNodes.emplace(m_pRoot->addr, m_pRoot);
    m_nodeNum++;
}

MemoryManager::~MemoryManager()
{
    //void* p = reinterpret_cast<void*>(m_addr);
    void* p = reinterpret_cast<void*>(m_pRoot->addr);
    printf("total node number in the tree: %d, %s() at %s:%d \n", m_nodeNum, __func__, __FILE__, __LINE__);
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
        if (p->pages == pages)
        {
            addr = p->addr;
            p->used = true;
            m_unusedNodes.erase(p->addr);
            printf("remove:0x%lx from dictionary, %s() at %s:%d \n",
               p->addr, __func__, __FILE__, __LINE__);

        }
        // split the node
        else if(p->pages > pages)
        {
            addr = p->addr;
            p->leaf = false;
            p->used = true;

            //we have to remove p before splitting the node, since left node inherits addr from parernt node
            m_unusedNodes.erase(p->addr);

            printf("removed node: %p, p->pages:%d, pages:%d, addr: 0x%lx,  %s() is called at %s:%d \n",
                   p, (int)p->pages, (int)pages,  p->addr, __func__, __FILE__, __LINE__);
            splitNode(p, pages);
        }
#if 1
        else
        {
            //check if we have continue memory which has chunk of size larger than pages
            auto it = m_unusedNodes.find(p->addr);
            if(it == m_unusedNodes.end())
            {
                printf("error, could not find unused node: %p, p->addr: 0x%lx, p->pages: %d, requested pages: %d,  %s() at %s:%d \n",
                       p, p->addr, (int)p->pages, (int)pages,   __func__, __FILE__, __LINE__);

                return;
            }
            auto oit = it;
            it++;

            int u = pages - p->pages;
            for(; it != m_unusedNodes.end(); oit = it, it++)
            {
                if(!it->second->used &&
                   oit->second->addr + oit->second->pages*m_PAGE_SIZE == it->second->addr )
                {
                    u -= it->second->pages;

                    if(u <= 0)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }

            if(u <= 0)
            {
                //there exists continuous chunk of memory
                addr = p->addr;
                p->used = true;
                int v = pages - p->pages;
                printf("node: %p marked as used, %s() at %s:%d \n", p, __func__, __FILE__, __LINE__);

                std::vector<unsigned long> iVec;
                iVec.push_back(p->addr);

                auto it = m_unusedNodes.find(p->addr);
                it++;

                Node* pNode = nullptr;
                for(; it != m_unusedNodes.end(); it++)
                {
                    iVec.push_back( it->second->addr);
                    it->second->used = true;

                    printf("node: %p marked as used, %s() at %s:%d \n", it->second, __func__, __FILE__, __LINE__);
                    v -= it->second->pages;

                    if(v == 0)
                    {
                        break;
                    }
                    else if(v < 0)
                    {
                        v += it->second->pages;
                        //splitNode(it->second, v);
                        pNode = it->second;
                        break;
                    }
                }

                for(auto key: iVec)
                {
                    m_unusedNodes.erase(key);
                    printf("remove:0x%lx added into dictionary, %s() at %s:%d \n",
                            key, __func__, __FILE__, __LINE__);
                }

                splitNode(pNode, v);
            }
        }
#endif
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
    p->addr = right->addr - 1; //as a key for binary search
    m_nodeNum +=2 ;

    printf("pages: %5d, n: %d, Node:%p is split into two sub nodes, left:%p(addr: 0x%lx), right:%p(addr: 0x%lx), %s() at %s:%d \n",
           (int)p->pages, (int)n, p,  left, left->addr,
           right, right->addr, __func__, __FILE__, __LINE__);

    left->used = true;
    if (n == left->pages)
    {
        m_unusedNodes.emplace(right->addr, right);

        printf("add:0x%lx added into dictionary, %s() at %s:%d \n",
               right->addr, __func__, __FILE__, __LINE__);
    }
    else if (n < left->pages)
    {
        m_unusedNodes.emplace(right->addr, right);
        printf("add:0x%lx added into dictionary, %s() at %s:%d \n",
               right->addr, __func__, __FILE__, __LINE__);

        splitNode(left, n);
    }
    else
    {
        right->used = true;
        splitNode(right, n - left->pages);
    }
}
void MemoryManager::deallocate(void* ptr)
{
    if(!ptr)
    {
        return;
    }
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
            // find the node where we allocated memory
            assert(p->used);
            p->used = false;
            m_unusedNodes[p->addr] = p;

            printf("node: %p marked as unused, %s() at %s:%d \n", p, __func__, __FILE__, __LINE__);

            if (pages > p->pages)
            {
                unsigned long addr = p->addr + p->pages * m_PAGE_SIZE;
                if (pp)
                {
                    deallocateMemory(nullptr, m_pRoot, addr, pages - p->pages);
                }
                else
                {
                    printf("Memrory management is messed up\n");
                }
            }
        }
    }

    printf("return path, node: %p, %s() at %s:%d \n", p, __func__, __FILE__, __LINE__);

    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    if (p->left != nullptr && p->left->used == false && p->right != nullptr && p->right->used == false)
    {
        // remove two child kids;
        p->addr = p->left->addr;
        p->pages = p->left->pages + p->right->pages;
        p->leaf = true;
        p->used = false;

        printf("two sub nodes are merged into one node, p:%p, left:%p, right:%p, pages:(p,l,r)=(%d, %d, %d), %s() at %s:%d \n",
               p, p->left, p->right, (int)p->pages, (int)p->left->pages, (int)p->right->pages,
               __func__, __FILE__, __LINE__);

        m_unusedNodes.erase(p->left->addr);
        printf("remove:0x%lx from dictionary, %s() at %s:%d \n",
               p->left->addr, __func__, __FILE__, __LINE__);

        m_unusedNodes.erase(p->right->addr);
        printf("remove:0x%lx from dictionary, %s() at %s:%d \n",
               p->right->addr, __func__, __FILE__, __LINE__);
        m_unusedNodes.emplace(p->addr, p);

        p->left->~Node();
        p->right->~Node();

        free(p->left);
        free(p->right);
        m_nodeNum -= 2;

    }
}

#if 0
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
    //unsigned long size = 4096 * 1024*256;
    unsigned long size = 4096 * 1024*256;

    MemoryManager memManager(size);

    std::vector<void*> memVec;

#if 0
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
#endif

    std::default_random_engine e;
    e.seed(1);
    std::uniform_int_distribution<unsigned> u(1, 32);

    int q = 2000;
    for(int i = 0; i < q; i++)
    {
        unsigned int sz = u(e);
        sz = sz*4096; //4096;
#ifdef DEBUG
        printf("memory allocation request, size in pages: %d \n",  (int)sz/4096);
        auto p = memManager.allocate(sz);
#else
        auto p = malloc(sz);
#endif

        if(p)
        {
            memVec.push_back(p);
        }

    }
    for (int i = 0; i < memVec.size(); i++)
    {
        //printf("memory address : %p \n", memVec[i]);
    }

    for (int i = 0; i < memVec.size(); i++)
    {
        printf("deallocate memory address : %p \n", memVec[i]);
#ifdef DEBUG
        memManager.deallocate(memVec[i]);
#else
        free(memVec[i]);
#endif
    }

    return 0;
}
