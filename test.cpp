// test.cpp
// Uncomment the next line to use precompiled headers
#include "pch.h"
// uncomment the next line  I did not use precompiled headers
// I used compiled the libary from the github then linked it with gcc
// Then I added the mandatory header
//#include "gtest/gtest.h"

// The global test environment setup and teardown
class Environment : public ::testing::Environment
{
public:
  ~Environment() override {}

  // Override this to define how to set up the environment.
  void SetUp() override
  {
    // Initialize random seed
    srand(time(nullptr));
  }

  // Override this to define how to tear down the environment.
  void TearDown() override {}
};

// Create our test class to house shared data between tests
class CollectionTest : public ::testing::Test
{
protected:
  // Create a smart pointer to hold our collection
  std::unique_ptr<std::vector<int>> collection;

  void SetUp() override
  { 
    // Create a new collection to be used in the test
    collection.reset(new std::vector<int>);
  }

  void TearDown() override
  { 
    // Erase all elements in the collection, if any remain
    collection->clear();
    // Free the pointer
    collection.reset(nullptr);
  }

  // Helper function to add random values from 0 to 99 count times to the collection
  void add_entries(int count)
  {
    assert(count > 0);
    for (auto i = 0; i < count; ++i)
      collection->push_back(rand() % 100);
  }
};

// Test that a collection is created successfully
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
  // Verify the collection is created
  ASSERT_TRUE(collection);
  // Verify the pointer is not null
  ASSERT_NE(collection.get(), nullptr);
}

// Test that a collection is empty when created
TEST_F(CollectionTest, IsEmptyOnCreate)
{
  // Verify the collection is empty
  ASSERT_TRUE(collection->empty());
  // Verify the size is 0
  ASSERT_EQ(collection->size(), 0);
}

// Test to see a failure in the test explorer
TEST_F(CollectionTest, AlwaysFail)
{
  FAIL();
}

// Test to verify adding a single value to an empty collection
TEST_F(CollectionTest, CanAddToEmptyVector)
{
  // Verify the collection is empty
  ASSERT_TRUE(collection->empty());
  ASSERT_EQ(collection->size(), 0);

  // Add a single entry
  add_entries(1);

  // Verify the collection is not empty
  ASSERT_FALSE(collection->empty());
  // Verify the size is 1
  ASSERT_EQ(collection->size(), 1);
}

// Test to verify adding five values to collection
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
  // Add five entries
  add_entries(5);
  
  // Verify the size is 5
  ASSERT_EQ(collection->size(), 5);
}

// Test to verify that max size is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, MaxSizeIsGreaterOrEqualToSize)
{
  std::vector<int>::size_type initial_size = collection->size();

  add_entries(1);
  EXPECT_GE(collection->max_size(), collection->size());

  add_entries(4);
  EXPECT_GE(collection->max_size(), collection->size());

  add_entries(5);
  EXPECT_GE(collection->max_size(), collection->size());
}

// Test to verify that capacity is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, CapacityIsGreaterOrEqualToSize)
{
  EXPECT_GE(collection->capacity(), collection->size());

  add_entries(1);
  EXPECT_GE(collection->capacity(), collection->size());

  add_entries(4);
  EXPECT_GE(collection->capacity(), collection->size());

  add_entries(5);
  EXPECT_GE(collection->capacity(), collection->size());
}

// Test to verify resizing increases the collection
TEST_F(CollectionTest, ResizingIncreasesCollection)
{
  collection->resize(10);
  EXPECT_EQ(collection->size(), 10);
}

// Test to verify resizing decreases the collection
TEST_F(CollectionTest, ResizingDecreasesCollection)
{
  add_entries(10);
  collection->resize(5);
  EXPECT_EQ(collection->size(), 5);
}

// Test to verify resizing decreases the collection to zero
TEST_F(CollectionTest, ResizingDecreasesCollectionToZero)
{
  add_entries(10);
  collection->resize(0);
  EXPECT_EQ(collection->size(), 0);
}

// Test to verify clear erases the collection
TEST_F(CollectionTest, ClearErasesCollection)
{
  add_entries(10);
  collection->clear();
  EXPECT_EQ(collection->size(), 0);
}

// Test to verify erase(begin,end) erases the collection
TEST_F(CollectionTest, EraseBeginEndErasesCollection)
{
  add_entries(10);
  collection->erase(collection->begin(), collection->end());
  EXPECT_EQ(collection->size(), 0);
}

// Test to verify reserve increases the capacity but not the size of the collection
TEST_F(CollectionTest, ReserveIncreasesCapacityNotSize)
{
  auto initial_size = collection->size();
  auto initial_capacity = collection->capacity();
  collection->reserve(100);
  EXPECT_GT(collection->capacity(), initial_capacity);
  EXPECT_EQ(collection->size(), initial_size);
}

// Test to verify the std::out_of_range exception is thrown when calling at() with an index out of bounds
TEST_F(CollectionTest, ThrowsOutOfRangeWhenAccessingInvalidIndex)
{
  add_entries(5);
  EXPECT_THROW(collection->at(10), std::out_of_range);
}

// Custom positive test: Verify that the collection can correctly store and retrieve values
TEST_F(CollectionTest, CanStoreAndRetrieveValues)
{
  add_entries(5);
  for (int i = 0; i < 5; ++i)
  {
    EXPECT_GE(collection->at(i), 0);
    EXPECT_LT(collection->at(i), 100);
  }
}

// Custom negative test: Verify that the collection throws an exception when accessing an invalid index after resizing
TEST_F(CollectionTest, ThrowsOutOfRangeAfterResizing)
{
  add_entries(5);
  collection->resize(3);
  EXPECT_THROW(collection->at(4), std::out_of_range);
}
