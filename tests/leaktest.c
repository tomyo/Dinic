#include "test_edge.h"
#include "test_network.h"
#include "test_list.h"
#include "test_queue.h"
#include "test_dinic.h"

int main(void){
    edge_memory_test();
    network_memory_test();
    list_memory_test();
    queue_memory_test();
    dinic_memory_test();
    return 0;
}
