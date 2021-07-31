#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WallG::Core;

namespace CoreTest
{
	TEST_CLASS(TypedAllocatorTest)
	{
	public:
		class TestClass
		{
		public:
			TestClass() : a(42), b(7) {};
			~TestClass() { a = 13; b = 5; };
			int a, b;
		};

		TEST_METHOD(TestNew)
		{
			TypedAllocator<TestClass> typedAllocator(1);
			TestClass* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);
			Assert::AreEqual(ptr->a, 42);
			Assert::AreEqual(ptr->b, 7);

			TestClass* ptr2 = typedAllocator.New();
			Assert::IsNull(ptr2);

		}

		TEST_METHOD(Test2)
		{
			TypedAllocator<TestClass> typedAllocator(2);
			TestClass* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);
			Assert::AreEqual(ptr->a, 42);
			Assert::AreEqual(ptr->b, 7);

			TestClass* ptr2 = typedAllocator.New();
			Assert::IsNotNull(ptr2);
			Assert::AreEqual(ptr2->a, 42);
			Assert::AreEqual(ptr2->b, 7);

		}

		TEST_METHOD(TestDelete)
		{
			TypedAllocator<TestClass> typedAllocator(1);
			TestClass* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);
			Assert::AreEqual(ptr->a, 42);
			Assert::AreEqual(ptr->b, 7);

			typedAllocator.Delete(ptr);
			Assert::AreEqual(ptr->a, 13);
			Assert::AreEqual(ptr->b, 5);

			TestClass* ptr2 = typedAllocator.New();
			Assert::IsTrue(ptr == ptr2);
			Assert::AreEqual(ptr2->a, 42);
			Assert::AreEqual(ptr2->b, 7);

		}

	};
}
