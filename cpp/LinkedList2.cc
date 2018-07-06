
typedef struct tNode
{
    int val;
    struct tNode* next;

} tNode;

tNode* circleInList(tNode* head)
{
    // x + y
    // u1, u2, ..., ux, v1, v2, ... vy
    // q advance 1, and p advance 2;
    // from u1 to v1, q need x hops; assume
    // q and p meets at vk, then we have
    // q has (x + k-1) hops, then p has travel
    // 2(x+k-1) = x + k -1 + ny

    // x + k -1 = ny;  where x + k -1 is known

    // travel from vk to v1 , we need y-k +1 hops for q
    // y - k + 1 +x +k - 1 = x + y;
    // y - k + 1 + cnt = x + y;

    if (head == nullptr || head->next == nullptr)
    {
        return nullptr;
    }

    tNode* p = head;
    tNode* q = head;

    bool hasCircle = false;
    while (true)
    {
        p = p->next;

        q = q->next;
        if (q == nullptr)
        {
            break;
        }
        q = q->next;

        if (p == nullptr || q == nullptr)
        {
            break;
        }
        else if (p == q)
        {
            hasCircle = true;
            break;
        }
    }

    if (hasCircle == false)
    {
        return nullptr;
    }

    int len = 1;
    p = p->next;
    while (p != q)
    {
        len++;
        p = p->next;
    }

    p = head;
    q = head;

    for (int i = 0; i < len; i++)
    {
        q = q->next;
    }

    while (true)
    {
        if (p == q)
        {
            break;
        }

        p = p->next;
        q = q->next;
    }

    return p;
}


int main()
{
    return 0;
}
