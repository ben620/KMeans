#ifndef KMEANS_H_2013_12_8
#define KMEANS_H_2013_12_8

#include <vector>
#include <algorithm>
#include <limits>

namespace algo{


template <class ForwardIterator>
const typename ForwardIterator::value_type 
	ComputeCenter(ForwardIterator begin, ForwardIterator end)
{
	using namespace std;
	if (begin == end)
	{
		return typename ForwardIterator::value_type();
	}

	typename ForwardIterator::value_type center;
	std::size_t nSize = 0;
	for_each(begin, end, [&center, &nSize](typename ForwardIterator::value_type &p)
		{
			center += p;
			++nSize;
		}
	);

	center /= nSize;
	return center;
}

template <class ForwardIterator>
void KMeans(ForwardIterator begin, ForwardIterator end, int K)
{
	using std::vector;
	typedef typename ForwardIterator::value_type::distance_type distance_type;
	typedef typename ForwardIterator::value_type::value_type value_type;

	vector<typename ForwardIterator::value_type> vSeeds(K);
	generate_n(vSeeds.begin(), K, [&]()
		{
			return typename ForwardIterator::value_type(value_type(rand() % 100), value_type(rand() % 100));
		}
	);

	for_each(begin, end, [](typename ForwardIterator::value_type &pt)
	{
		pt.ID = 0;
	}
	);


	while (1)
	{
		//compute group
		for (ForwardIterator iterPoint = begin; iterPoint != end; ++iterPoint)
		{
			distance_type minDist = (std::numeric_limits<distance_type>::max)();
			int minDistIdx = 0;

			for (int seedIdx = 0; seedIdx < K; ++seedIdx)
			{
				distance_type dist = BlkDist(*iterPoint, vSeeds[seedIdx]);
				if (dist < minDist)
				{
					minDist = dist;
					minDistIdx = seedIdx;
				}
			}

			iterPoint->ID = minDistIdx;
		}

		//find center seed
		vector<typename ForwardIterator::value_type> vTmpSeeds(K);
		for (ForwardIterator iterPoint = begin; iterPoint != end; ++iterPoint)
		{
			vTmpSeeds[iterPoint->ID] += *iterPoint;
			++vTmpSeeds[iterPoint->ID].ID;
		}

		distance_type seedDist = 0;
		for (int ii = 0; ii < K; ++ii)
		{
			if (vTmpSeeds[ii].ID == 0)
			{
				continue;
			}

			vTmpSeeds[ii] /= vTmpSeeds[ii].ID;
			seedDist += BlkDist(vSeeds[ii], vTmpSeeds[ii]);
		}
		if (seedDist < 1.0f)
		{
			goto END;
		}
		vSeeds = vTmpSeeds;
	}

END:
	return;
}

};

#endif