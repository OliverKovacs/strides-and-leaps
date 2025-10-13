#include <cassert>
#include <cstdint>
#include <deque>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <ranges>

#define OPADD 0
#define OPSUB 1
#define OPDIV 1

/**
 * @param g generator
 * @param m modulus
 */
static void cyclic_subgroup(int32_t g, int32_t m, std::set<int32_t> *s)
{
    assert(s->empty());

    g = ((g % m) + m) % m;
    int32_t b = g;
    do {
        s->insert(b);
        b = (b * g) % m;
    } while (b != g);
}

struct step
{
    unsigned int op : 1;
    unsigned int sign : 1;
    int prev : 30;
};

struct problem
{
    int32_t k;
    int32_t n;
    int32_t goal;
    int32_t min;
    int32_t max;
    std::map<int32_t, int32_t> *steps;
    std::map<int32_t, int32_t> *prevs;

    /**
     * @param k as defined by the problem statement
     * @param n maximum search depth
     * @param goal number to be reached
     */
    problem(int32_t k, int32_t n, int32_t goal) {
        this->k = k;
        this->n = n;
        this->goal = goal;
        this->min = 0;
        this->max = std::numeric_limits<int32_t>::max();
        this->steps = new std::map<int32_t, int32_t>();
        this->prevs = new std::map<int32_t, int32_t>();
    }

    ~problem() {
        delete this->steps;
        delete this->prevs;
    }

    static int32_t next(struct step *s, int32_t k)
    {
        return s->op == OPDIV
            ? s->prev * 2
            : s->prev + k * (s->sign == OPADD ? 1 : -1);
    }

    static void solve(struct problem *p)
    {
        // init
        (*p->steps)[p->goal] = 0;
        (*p->prevs)[p->goal] = p->goal;

        std::deque<step> Q;
        Q.push_back(step(OPDIV, 0, p->goal));
        Q.push_back(step(OPADD, OPADD, p->goal));
        Q.push_back(step(OPADD, OPSUB, p->goal));

        // bfs
        while (!Q.empty()) {
            struct step s = Q.front();
            Q.pop_front();

            int32_t curr = next(&s, p->k);

            // suboptimal
            if (p->steps->contains(curr) && (*p->steps)[curr] <= (*p->steps)[s.prev])
                continue;

            // outside range
            if (curr < p->min || p->max < curr)
                continue;

            (*p->steps)[curr] = (*p->steps)[s.prev] + 1;
            (*p->prevs)[curr] = s.prev;

            if ((*p->steps)[curr] >= p->n)
                continue;

            Q.push_back(step(OPDIV, 0, curr));

            // if ((curr > 1 || curr < -1) && s.op == OPADD)
            //     continue;

            // if (curr & 1)
            //     continue;

            Q.push_back(step(OPADD, OPADD, curr));
            Q.push_back(step(OPADD, OPSUB, curr));
        }
    }

    static void path(struct problem *p, int32_t x, std::vector<int32_t> *v)
    {
        if (!p->prevs->contains(x))
            return;

        while ((*p->prevs)[x] != x) {
            v->push_back(x);
            x = (*p->prevs)[x];
        }
        v->push_back(x);
    }

    static void print(struct problem *p)
    {
        for (const auto &e : *p->steps)
            std::cout << e.first
                << " " << (*p->steps)[e.first]
                << " " << (*p->prevs)[e.first] << "\n";

        std::cout.flush();
    }
};

void print_vector(std::vector<int32_t> *v) {
    if (!v->empty())
        std::cout << (*v)[0];

    for (const auto e : *v | std::views::drop(1))
        std::cout << " " << e;

    std::cout << std::endl;
}

// TODO
void print_set(std::set<int32_t> *v) {
    if (!v->empty())
        std::cout << *(v->begin());

    for (const auto e : *v | std::views::drop(1))
        std::cout << " " << e;

    std::cout << std::endl;
}

int main ()
{
    int32_t k;
    int32_t n;
    int32_t goal;
    std::cin >> k >> n >> goal;

    struct problem *p = new problem(k, n, goal);
    problem::solve(p);
    problem::print(p);
    // std::cout << std::endl;

    std::string str;
    while (true) {
        std::cin >> str;
        if (str == "x")
            break;

        int32_t start = std::stoi(str);
        std::vector<int32_t> path = {};
        problem::path(p, start, &path);

        if (path.empty()) {
            std::cout << start << " not reachable in "
                << p->n << " steps" << std::endl;
            continue;
        }

        print_vector(&path);
    }

    delete p;
}
