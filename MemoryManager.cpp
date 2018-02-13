#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>

class MemoryManager
{
private:
    struct Node
    {
        Node* left;
        Node* right;
        long addr;
        bool leaf;
        bool used;
        size_t size;

        Node()
        {
            left = nullptr;
            right = nullptr;
            addr = 0;
            leaf = true;
            used = false;
            size = 0;
        }
        ~Node() {

            //printf("%s() is called at %s:%d \n", __func__, __FILE__, __LINE__);

        }
    };

public:
    MemoryManager(size_t sz)
    {
        int n = sz / m_PAGE_SIZE;
        int r = sz % m_PAGE_SIZE;
        r = r ? 1 : 0;
        n = (n + r) * m_PAGE_SIZE;

        void* p = malloc(n);
        m_addr = reinterpret_cast<long>(p);
        m_size = n;

        m_pRoot = new Node();
        m_pRoot->size = n;
        m_pRoot->addr = m_addr;
    }

    ~MemoryManager()
    {
        void* p = reinterpret_cast<void*>(m_pRoot->addr);
        free(p);
        m_pRoot = nullptr;
    }
    void* allocate(size_t sz)
    {
        Node* pNode = nullptr;
        allocateMemory(m_pRoot, sz, pNode);
        //printf("%s() is called at %s:%d \n", __func__, __FILE__, __LINE__);

        if (pNode != nullptr)
        {
            return reinterpret_cast<void*>(pNode->addr);
        }
    }

    void allocateMemory(Node* p, size_t sz, Node*& rt)
    {
        if (!p || rt)
        {
            return;
        }
        //printf("%s() is called at %s:%d \n", __func__, __FILE__, __LINE__);

        if (!p->leaf)
        {
            //printf("%s() is called at %s:%d \n", __func__, __FILE__, __LINE__);

            allocateMemory(p->left, sz, rt);
            allocateMemory(p->right, sz, rt);
        }
        else
        {
            if (p->used)
            {
                return;
            }
            //printf("%s() is called at %s:%d \n", __func__, __FILE__, __LINE__);
            // split the node
            int n = sz / m_PAGE_SIZE;
            int r = sz % m_PAGE_SIZE;
            r = r ? 1 : 0;
            n = (n + r) * m_PAGE_SIZE;

            if (p->size - n > 0)
            {
                Node* left = new Node();
                Node* right = new Node();

                left->addr = p->addr;
                left->size = n;

                right->addr = p->addr + n;
                right->size = p->size - n;

                p->leaf = false;
                left->used = true;

                p->left = left;
                p->right = right;

                rt = left;
            }
        }
    }

    void deallocate(const void* ptr) { deallocateMemory(m_pRoot, reinterpret_cast<long>(ptr)); }

    void deallocateMemory(Node* p, long addr)
    {
        if (!p)
        {
            return;
        }

        if (!p->leaf)
        {
            if (addr <= p->addr)
            {
                deallocateMemory(p->left, addr);
            }
            else
            {
                deallocateMemory(p->right, addr);
            }
        }
        else
        {
            if (p->addr == addr)
            {
                // find the node we allocated the memory from it
                assert(p->used);
                p->used = false;
            }
        }

        if (p->left != nullptr && p->left->used == false && p->right != nullptr && p->right->used == false)
        {
            // remove two child kids;
            p->addr = p->left->addr;
            p->size = p->left->size + p->right->size;

            p->leaf = true;
            p->used = false;

            delete p->left;
            delete p->right;
        }
    }

private:
    Node* m_pRoot;
    long m_addr;
    size_t m_size;
    static const size_t m_PAGE_SIZE = 4096;
};

int main()
{
    int size = 4096 * 1024;
    MemoryManager memManager(size);

    std::vector<void*> memVec;
    int n = 128;
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
