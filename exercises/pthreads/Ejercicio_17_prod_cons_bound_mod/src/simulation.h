#include "producer.h"
#include "consumer.h"
#include "thread_data.h"

int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
int create_threads(shared_data_t* shared_data);