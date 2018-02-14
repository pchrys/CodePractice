#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>
#include <cstring>

#include "MemoryManager.h"

const size_t MemoryManager::m_PAGE_SIZE;

MemoryManager::MemoryManager(size_t sz)
{
    int n = sz / m_PAGE_SIZE;
    int r = sz % m_PAGE_SIZE;
    r = r ? 1 : 0;
    int m = (n + r) * m_PAGE_SIZE;

    void* p = malloc(n);
    memset(p, 0x0, n);

    m_addr = reinterpret_cast<unsigned long>(p);
    m_size = m;

    m_pRoot = new Node();
    m_pRoot->size = n;
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

        printf("p= %p, %s() is called at %s:%d \n", p, __func__, __FILE__, __LINE__);

        return p;
    }

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
        printf("pages:%d, %s() is called at %s:%d \n", (int)pages,  __func__, __FILE__, __LINE__);
        allocateMemory(p->left, pages, addr);
        allocateMemory(p->right, pages, addr);
    }
    else
    {
        if (p->used)
        {
            printf("p->pages:%d, pages:%d, node:%p,  %s() is called at %s:%d \n",
                   (int)p->pages, (int)pages,  p, __func__, __FILE__, __LINE__);
            return;
        }

        // split the node
        if (p->pages == pages)
        {
            addr = p->addr;
            p->used = true;
        }
        else if(p->pages > pages)
        {
            printf("p->pages:%d, pages:%d, node:%p,  %s() is called at %s:%d \n",
                   (int)p->pages, (int)pages,  p, __func__, __FILE__, __LINE__);

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

    Node* left = new Node();
    Node* right = new Node();
    left->pages = p->pages / 2;
    right->pages = p->pages - left->pages;

    left->addr = p->addr;
    right->addr = p->addr + left->pages * m_PAGE_SIZE;

    p->leaf = false;
    p->used = true;
    p->left = left;
    p->right = right;
    p->addr = right->addr;

    printf("pages: %5d, Node:%p is split into two sub nodes, left:%p(addr:%lx), right:%p(addr:%lx), %s() at %s:%d \n",
           (int)p->pages, p,  left, left->addr,
           right, right->addr, __func__, __FILE__, __LINE__);

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
    printf("pages: %d,  p: %p,  %s() at %s:%d \n",
           (int)pages, p,   __func__, __FILE__, __LINE__);
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
        printf("pages: %d, release node:%p, p->addr:0x%lx, addr:0x%lx  %s() at %s:%d \n",
               (int)p->pages, p, p->addr, addr,  __func__, __FILE__, __LINE__);

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

    if (p->left != nullptr && p->left->used == false && p->right != nullptr && p->right->used == false)
    {
        // remove two child kids;
        p->addr = p->left->addr;
        p->pages = p->left->pages + p->right->pages;
        p->leaf = true;
        p->used = false;

        printf("two sub nodes are merged into one node, %s() at %s:%d \n",
               __func__, __FILE__, __LINE__);
        delete p->left;
        delete p->right;
    }
}

int main()
{
    int size = 4096 * 1024;
    MemoryManager memManager(size);

    std::vector<void*> memVec;
    int n = 10;
    for (int i = 0; i < n; i++)
    {
        auto p = memManager.allocate(4096);
        printf("memory address : %p \n", p);
        memVec.push_back(p);
    }

    for (int i = 0; i < memVec.size(); i++)
    {
        printf("memory address : %p \n", memVec[i]);
    }

    for (int i = 0; i < memVec.size(); i++)
    {
        //printf("deallocate memory address : %p \n", memVec[i]);
        memManager.deallocate(memVec[i]);
    }
}
