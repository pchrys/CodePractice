#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>

#include "MemoryManager.h"

const size_t MemoryManager::m_PAGE_SIZE;

MemoryManager::MemoryManager(size_t sz)
{
    int n = sz / m_PAGE_SIZE;
    int r = sz % m_PAGE_SIZE;
    r = r ? 1 : 0;
    int m = (n + r) * m_PAGE_SIZE;

    void* p = malloc(n);
    m_addr = reinterpret_cast<long>(p);
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
    Node* pNode = nullptr;
    size_t sz1 = sz + sizeof(long);

    int n = sz1 / m_PAGE_SIZE;
    int r = sz1 % m_PAGE_SIZE;
    r = r ? 1 : 0;
    int m = (n + r);

    allocateMemory(m_pRoot, m, pNode);

    if (pNode != nullptr)
    {
        void* p = reinterpret_cast<void*>(pNode->addr);
        long* q = reinterpret_cast<long*>(p);
        *q = m;
        p = reinterpret_cast<void*>(reinterpret_cast<long>(p) + sizeof(long));

        printf("p= %p, %s() is called at %s:%d \n", p, __func__, __FILE__, __LINE__);

        return p;
    }

    return nullptr;
}

void MemoryManager::allocateMemory(Node* p, size_t pages, Node*& rt)
{

    if (!p || rt)
    {
        return;
    }

    if (!p->leaf)
    {
        printf("pages:%d, %s() is called at %s:%d \n", (int)pages,  __func__, __FILE__, __LINE__);

        allocateMemory(p->left, pages, rt);
        allocateMemory(p->right, pages, rt);
    }
    else
    {
        if (p->used)
        {
            return;
        }
        // printf("%s() is called at %s:%d \n", __func__, __FILE__, __LINE__);
        // split the node

        if (p->pages > pages)
        {
            printf("p->pages:%d, pages:%d, node:%p,  %s() is called at %s:%d \n",
                   (int)p->pages, (int)pages,  p, __func__, __FILE__, __LINE__);

            rt = p;
            splitNode(p, pages);
        }
    }
}

void MemoryManager::splitNode(Node* p, size_t n)
{
    if (!p || p->pages < n || p->pages <= 1)
    {
        return;
    }


    Node* left = new Node();
    Node* right = new Node();
    left->pages = p->pages / 2;
    right->pages = p->pages - left->pages;

    left->addr = p->addr;
    right->addr = p->addr + left->addr * m_PAGE_SIZE;
    p->leaf = false;
    p->used = true;
    p->left = left;
    p->right = right;

    printf("pages: %d, Node is split into two sub nodes, left:%p, right:%p, %s() at %s:%d \n",
           (int)p->pages, left, right, __func__, __FILE__, __LINE__);


    if (n == left->pages)
    {
        left->used = true;
        return;
    }

    if (n < left->pages)
    {
        splitNode(left, n);
    }
    else
    {
        left->used = true;
        splitNode(right, n - left->pages);
    }
}
void MemoryManager::deallocate(const void* ptr)
{
    void* p = reinterpret_cast<void*>(reinterpret_cast<long>(ptr) - sizeof(long));
    long* q = reinterpret_cast<long*>(p);
    size_t pages = *q;

    deallocateMemory(nullptr, m_pRoot, reinterpret_cast<long>(p), pages);
}

void MemoryManager::deallocateMemory(Node* pp, Node* p, long addr, size_t pages)
{
    if (!p)
    {
        return;
    }

    if (!p->leaf)
    {
        if (addr <= p->addr)
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
        if (p->addr == addr)
        {
            // find the node we allocated the memory from it
            assert(p->used);
            p->used = false;

            if (pages > p->pages)
            {
                long addr = p->addr + p->pages * m_PAGE_SIZE;
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
        p->size = p->left->size + p->right->size;
        p->leaf = true;
        p->used = false;

        printf("two sub nodes are merged into one node, %s() at %s:%d \n", __func__, __FILE__, __LINE__);
        delete p->left;
        delete p->right;
    }
}

int main()
{
    int size = 4096 * 1024;
    MemoryManager memManager(size);

    std::vector<void*> memVec;
    int n = 2;
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
        memManager.deallocate(memVec[i]);
    }
}
