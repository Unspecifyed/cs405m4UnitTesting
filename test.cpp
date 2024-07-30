#include <cstdlib>
#include <ctime>
#include "gtest/gtest.h"
#include <vector>
#include <memory>
#include <stdexcept>
#include <openssl/rand.h>

// Function to generate secure random bytes
void generate_secure_random_bytes(unsigned char* buffer, int length) {
    if (RAND_bytes(buffer, length) != 1) {
        throw std::runtime_error("Error generating secure random bytes.");
    }
}

// The global test environment setup and teardown
class Environment : public ::testing::Environment
{
public:
  ~Environment() override {}

  void SetUp() override
  {
    // No specific setup needed for OpenSSL random number generation
  }

  void TearDown() override {}
};

// Create our test class to house shared data between tests
class CollectionTest : public ::testing::Test
{
protected:
  std::unique_ptr<std::vector<int>> collection;

  void SetUp() override
  { 
    collection = std::make_unique<std::vector<int>>();
  }

  void TearDown() override
  { 
    collection->clear();
    collection.reset(nullptr);
  }

  // Helper function to add secure random values from 0 to 99, count times, to the collection
  void add_entries(int count)
  {
    assert(count > 0);
    unsigned char random_byte;
    for (int i = 0; i < count; ++i) {
      generate_secure_random_bytes(&random_byte, 1);
      collection->push_back(random_byte % 100);
    }
  }
};

// Test that a collection is created successfully
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
  ASSERT_TRUE(collection);
  ASSERT_NE(collection.get(), nullptr);
}

// Test that a collection is empty when created
TEST_F(CollectionTest, IsEmptyOnCreate)
{
  ASSERT_TRUE(collection->empty());
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
  ASSERT_TRUE(collection->empty());
  ASSERT_EQ(collection->size(), 0);

  add_entries(1);

  ASSERT_FALSE(collection->empty());
  ASSERT_EQ(collection->size(), 1);
}

// Test to verify adding five values to collection
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
  add_entries(5);
  ASSERT_EQ(collection->size(), 5);
}

// Test to verify that max size is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, MaxSizeIsGreaterOrEqualToSize)
{
  EXPECT_GE(collection->max_size(), collection->size());

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
