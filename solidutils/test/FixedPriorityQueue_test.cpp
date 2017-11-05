/**
 * @file FixedPriorityQueue_test.cpp
 * @brief Unit tests for the FixedPriorityQueue class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017, Solid Lake LLC
 * @version 1
 * @date 2017-10-20
 */



#include "UnitTest.hpp"
#include "FixedPriorityQueue.hpp"


namespace sl
{


UNITTEST(FixedPriortyQueue, AddPopInOrder)
{
  FixedPriorityQueue<float, int> pq(0, 10);

  for (int i = 0; i < 10; ++i) {
    pq.add(1.0/(i+1), i);
  }

  for (int i = 0; i < 10; ++i) {
    int const num = pq.pop();
    testEqual(num, i);
  }
}


UNITTEST(FixedPriortyQueue, AddPeek)
{
  FixedPriorityQueue<float, int> pq(0, 10);

  for (int i = 0; i < 10; ++i) {
    pq.add(1.0/(i+1), i);
  }

  testEqual(pq.peek(), 0);
}


UNITTEST(FixedPriortyQueue, AddMax)
{
  FixedPriorityQueue<float, int> pq(0, 10);

  for (int i = 0; i < 10; ++i) {
    pq.add(1.0/(i+1), i);
  }

  testEqual(pq.max(), 1.0);
}


UNITTEST(FixedPriortyQueue, AddPopReverseOrder)
{
  FixedPriorityQueue<float, int> pq(0, 10);

  for (int i = 0; i < 10; ++i) {
    pq.add(i/10.0, i);
  }

  for (int i = 10; i > 0;) {
    --i;
    int const num = pq.pop();
    testEqual(num, i);
  }
}


UNITTEST(FixedPriortyQueue, Clear)
{
  FixedPriorityQueue<float, int> pq(0, 10);

  for (int i = 0; i < 10; ++i) {
    pq.add(1.0/(i+1), i);
  }

  pq.clear();
  testEqual(0, pq.size());

  // test that we can re-use the priorty queue again
  for (int i = 0; i < 10; ++i) {
    pq.add(i/10.0, i);
  }

  for (int i = 10; i > 0;) {
    --i;
    int const num = pq.pop();
    testEqual(num, i);
  }

}


}
