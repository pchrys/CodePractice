#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <mutex>
#include <map>
#include <unordered_map>
/**
 * this is a class used to perform memory management.

The idea is that we use a binary searech tree to manage the memory.
The leaf node of the tree keeps track of memory usage. Internal nodes
don't have those information.

When we allocate new memory, we need first find an unused leaf node, and
then check memory chunk size, if it is equal to the request size, then
return the memory denoted by this node; if it is larger than the request
size, we need equally split the current node into two sub notes, and check
left sub nodes; if the requested size is equal to the left node, return; if
it is smaller than the left node, continue recursively splitting left node;
if it is larger then left node, then search right sub-node for remaing size;

If the leaf node is smaller than requested size,  we need search if we have
free contious memory starting from address p->addr,  which in size is larger
than requested size, if yes, then allocate memory from this chunk; otherwise,
skip to next section



We skip unused leaf nodes which has less memory chunk than we request.

*/


//An imrpovement,

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
        size_t pages;

        Node()
        {
            left = nullptr;
            right = nullptr;
            addr = 0;
            leaf = true;
            used = false;
            pages = 0;
        }

        ~Node()
        {
            left = nullptr;
            right = nullptr;
            addr = 0;
            leaf = false;
            used = false;
            pages = 0;

            // printf("%s() is called at %s:%d \n", __func__, __FILE__, __LINE__);
        }
    };

public:
    MemoryManager(size_t sz);
    ~MemoryManager();
    MemoryManager(const MemoryManager& obj) = delete; // prevent copy
    void* allocate(size_t sz);
    void deallocate(void* ptr);

private:
    void allocateMemory(Node* p, size_t sz, unsigned long& addr);
    void allocateMemory(size_t pages, unsigned long& addr);
    void deallocateMemory(Node* pp, Node* p, unsigned long addr, size_t pages);
    void splitNode(Node* p, size_t n);

    Node* m_pRoot;
    int m_nodeNum = 0;
    unsigned long m_addr;
    static const size_t m_PAGE_SIZE = 4096;

    //we may use hash table to optimize performance
    std::map<unsigned long, Node*> m_unusedNodes;
    std::recursive_mutex m_mutex;
};

#endif
