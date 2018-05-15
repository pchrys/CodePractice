#include <memory>
#include <cstdio>
#include <random>
#include <thread>
#include <chrono>
#include <algorithm>
#include <utility>


template <typename T>
class tBinaryTree
{
public:
    tBinaryTree() {}
    bool Add(const T& v) { return Add(nullptr, m_xRoot.get(), v); }
    bool Remove(const T& v) { return Remove(nullptr, m_xRoot.get(), v); }
    void InOrderTraverse()
    {
        //printf("\n===============InOrderWalker==================\n");
        InOrderTraverse(m_xRoot.get());
        //printf("\n");
    }

    struct tNode
    {
        T key{0};
        std::unique_ptr<tNode> left;
        std::unique_ptr<tNode> right;

        // tNode() { printf("key= %4d, %s() is called at %s:%d \n", key, __func__, __FILE__, __LINE__); }

        // we need remove destructor in order to use default move constructor/move assignment
        // operator()
        //~tNode() { printf("key = %4d, %s() is called at %s:%d \n", key, __func__, __FILE__, __LINE__); }
    };
    // struct tNode;

    std::pair<bool, tNode*> Search(const T& v) { return Search(m_xRoot.get(), v); }

private:
    bool Add(tNode* p, tNode* q, const T& v)
    {
        if (q == nullptr)
        {
            if (p == nullptr)
            {
                m_xRoot = std::make_unique<tNode>();
                m_xRoot->key = v;
                return true;
            }
            else
            {
                if (v < p->key)
                {
                    p->left = std::make_unique<tNode>();
                    p->left->key = v;
                    return true;
                }
                else if (v > p->key)
                {
                    p->right = std::make_unique<tNode>();
                    p->right->key = v;
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        if (v < q->key)
        {
            return Add(q, q->left.get(), v);
        }
        else if (v > q->key)
        {
            return Add(q, q->right.get(), v);
        }
        else
        {
            return false;
        }
    }

    bool Remove(tNode* p, tNode* q, const T& v)
    {
        if (q == nullptr)
        {
            return false;
        }

        if (q->key == v)
        {
            if (!q->left || !q->right)
            {
                if (p)
                {
                    if (p->left.get() == q)
                    {
                        p->left = q->left ? std::move(q->left) : std::move(q->right);
                    }
                    else
                    {
                        p->right = q->left ? std::move(q->left) : std::move(q->right);
                    }
                }
                else
                { // remove root, and we need set root to new node
                    m_xRoot = q->left ? std::move(q->left) : std::move(q->right);
                }

                return true;
            }
            else
            { // both children are not zero
                auto r = q->left.get();
                auto rp = p;

                while (r)
                {
                    rp = r;
                    r = r->right.get();
                }

                // copy key from rp to q;
                q->key = rp->key;
                return Remove(q, q->left.get(), rp->key);
            }
        }
        else if (v < q->key)
        {
            return Remove(q, q->left.get(), v);
        }
        else
        {
            return Remove(q, q->right.get(), v);
        }
    }

    std::pair<bool, tNode*> Search(tNode* p, const T& v)
    {
        if (p == nullptr)
        {
            return std::make_pair(false, p);
        }

        if (p->key == v)
        {
            return std::make_pair(true, p);
        }
        else if (v < p->key)
        {
            return Search(p->left.get(), v);
        }
        else
        {
            return Search(p->right.get(), v);
        }
    }
    void InOrderTraverse(tNode* p)
    {
        if (p == nullptr)
        {
            return;
        }

        InOrderTraverse(p->left.get());
        //printf("%4d ", p->key);
        InOrderTraverse(p->right.get());
    }

    std::unique_ptr<tNode> m_xRoot;
};


int main()
{
    tBinaryTree<int32_t> tree;

    std::default_random_engine e(31);

    size_t n = 20;
    std::uniform_int_distribution<int32_t> d(-2 * n, 2 * n);
    std::vector<int32_t> ivec;
    for (size_t i = 0; i < n; i++)
    {
        // tree.Add(d(e));
        ivec.push_back(d(e));
    }

    std::shuffle(ivec.begin(), ivec.end(), std::default_random_engine());

    // for (auto& e : ivec)
    // {
    //     printf("%4d ", e);
    // }
    // printf("\n");

    for (auto& e : ivec)
    {
        tree.Add(e);
    }

    tree.InOrderTraverse();

    for (int i = 0; i < ivec.size() / 2; i++)
    {
        tree.Remove(ivec[i]);
    }

    tree.InOrderTraverse();

    // while (true)
    // {
    //     std::this_thread::sleep_for(std::chrono::seconds(2));
    // }


    for(auto& e : ivec){
        auto r = tree.Search(e);
    }

    //printf("r.first: %d, r.second->key: %d \n", r.first, r.second->key);

    return 0;
}
