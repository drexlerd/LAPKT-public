#include "utils.hxx"

namespace aptk {
namespace features {

int addition(int a, int b) {
    if (a == INF || b == INF) {
        return INF;
    }
    return a + b;
}

}
}
