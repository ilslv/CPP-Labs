#include "stdafx.h"
#include "CppUnitTest.h"
#include "../MatrixMultithreaded/Matrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
		template<typename T>
		static bool are_matrices_equal(matrix<T> m1, matrix<T> m2, const int x, const int y)
		{
			for (auto i = 0; i < x; ++i)
			{
				for (auto j = 0; j < y; ++j)
				{
					if(m1[i][j] != m2[i][j])
						return false;
				}
			}
			return true;
		}
	public:
		
		TEST_METHOD(TestMethod1)
		{
			matrix<double> m1(1, 1, 4);
			matrix<double> m2(1, 1, 3);
			Assert::IsFalse(are_matrices_equal(m1*m2, matrix<double>(1, 1, 12), 1, 1));
		}

	};
}