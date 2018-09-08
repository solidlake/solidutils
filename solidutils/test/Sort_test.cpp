/**
* @file Sort_test.cpp
* @brief Unit tests for the Sort class.
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-08-23
*/


#include "Sort.hpp"
#include "UnitTest.hpp"

#include <random>

namespace sl
{

UNITTEST(Sort, FixedKeys)
{
  std::vector<int> keys{0,5,2,3,4,1,1,3};

  std::unique_ptr<size_t[]> perm = Sort::fixedKeys<int, size_t>(keys.data(), \
      keys.size());
  
  testEqual(perm[0], 0UL);
  testEqual(perm[1], 5UL);
  testEqual(perm[2], 6UL);
  testEqual(perm[3], 2UL);
  testEqual(perm[4], 3UL);
  testEqual(perm[5], 7UL);
  testEqual(perm[6], 4UL);
}

UNITTEST(Sort, FixedKeysRandom)
{
  std::vector<int> keys{0,1,0,0,2,1,0,1,2,2,1,0,1,2,2,1,1,1};

  std::mt19937 rng(0);

  std::unique_ptr<size_t[]> perm1 = Sort::fixedKeysRandom<int, size_t>( \
      keys.data(), keys.size(), rng);
  std::unique_ptr<size_t[]> perm2 = Sort::fixedKeysRandom<int, size_t>( \
      keys.data(), keys.size(), rng);

  // check order
  for (size_t i = 0; i+1 < keys.size(); ++i) {
    testLessOrEqual(keys[perm1[i]], keys[perm1[i+1]]);
    testLessOrEqual(keys[perm2[i]], keys[perm2[i+1]]);
  }

  // make sure orders are different
  int different = 0;
  for (size_t i = 0; i < keys.size(); ++i) {
    if (perm1[i] != perm2[i]) {
      ++different;
    }
  }
  testGreater(different, 0);
}



}
