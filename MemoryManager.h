#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

class MemoryManager
{
private:
    struct Node
    {
        Node* left;
        Node* right;
        unsigned long addr;
        bool leaf;
        bool used;
        size_t size;
        size_t pages;

        Node()
        {
            left = nullptr;
            right = nullptr;
            addr = 0;
            leaf = true;
            used = false;
            pages = 0;
            size = 0;
        }
        ~Node() {

            //printf("%s() is called at %s:%d \n", __func__, __FILE__, __LINE__);

        }
    };

public:
    MemoryManager(size_t sz);
    ~MemoryManager();
    void* allocate(size_t sz);
    void deallocate(void* ptr);

private:
    void allocateMemory(Node* p, size_t sz, unsigned long& addr);
    void deallocateMemory(Node*pp, Node* p, unsigned long addr, size_t pages);
    void splitNode(Node*p, size_t n);

    Node* m_pRoot;
    unsigned long m_addr;
    size_t m_size;
    static const size_t m_PAGE_SIZE = 4096;
};

#endif
