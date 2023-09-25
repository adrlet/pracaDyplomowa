#include <stddef.h>

#include "asymmetric_algorithm_map.h"

const char * asymmetric_algorithms[] =
{
    "EC",
    "DSA",
    "DH",
    NULL
};

const char * asymmetric_sign_algorithms[] =
{
    "EC",
    "DSA",
    NULL
};

const char ** asymmetric_get_algorithms()
{
    return asymmetric_algorithms;
}

const char ** asymmetric_get_sign_algorithms()
{
    return asymmetric_sign_algorithms;
}
