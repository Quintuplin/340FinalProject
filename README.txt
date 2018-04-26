CSCI 340 Final Project
Jack Fraser & Anthony Morrell
4.25.2018
Dr Leclerc

1. 
	How big is a cache block?
		Experimentally, we found that 128 was an extremely reliable value, tested by finding the at which increasing to the next power of 2 
		(256) had the greatest % increase in time per request. While this value was only a tiny bit larger than it's neighbors, the result was
		unique among all our tests in that it always reached the same exact value, for all machines, and under all circumstances. So either we
		did something very very right, or something very very wrong.
		
		8) How we tested for it:
			
		
2. 
	How big is the cache?
		Finding the three levels of cache posed a significantly harder question. In fact, even with iterations running until the very last few
		hours of the many times extended deadline, we still do not have a method that reliably finds the correct values (as found in hardware
		specs). However, correllating inversely to the inaccuracy of our test results is the complexity and quality of our tests; 
		
		7) How we tested for it:
			
		
3.
	How long does a reference to main memory take to complete?
		This is a much easier question to answer, although not as much as the last question. Mostly, this one was solved by making a big array,
		then iterating through it in big leaps to always miss the cache. Add a little extra tests, and some reallocations to make sure the cache
		(of unknown total size and still uncertain block size) has minimal to no hits. All in all, our solution seems decent, and while perhaps
		not perfect, absolutely hits the ballpark.
		
		6) How we tested for it:
			A variant on the cache solution, the noncache solution had but two main differences.
				a) a much larger array was malloced, to ensure that it would not be in cache to begin with. This array was also freed and re-
					allocated between trials, so that each test would be equally cache-free.
				b) a custom spacing was figured out for the for loop. This spacing would be sufficiently large as to dodge even the largest of 
					possible cache and cache-block sizes, while being as small as possible within that parameter to allow for the maximum number
					of tests before a reallocation would be required
			Beyond that, t'was yet again not much more than a big nested loop set; with the timing element outside of the innermost loop to allow
			even that small error to be statistically minimized.
4.
	How long does a reference that can be satisfied from cache take to complete?
		The last question on this sheet, but the first that we answered with code. This was by far the easiest value to test for, and in many ways
		the only value which we can be certain is correct. The simplest to write, the simplest to understand... at least we got one of the four, eh?
		
		5) How we tested for it:
			First, we allocate an array of size 1. It could have been an int, or a char, but we went with an array. Then we start a timer, access 
			the value stored at a[0] a large number of times, then stop the timer and divide our result by the number of loops. This method allowed
			us to minimize the error as much as possible; while the for loop has some overhead, and the ++ command some as well, an extremely large 
			source of error is the startup and stopping of the timer itself. By pushing this outside the innermost loop, we allowed that error to be
			minimized to a near negligible amount.
			After that is was simply taking the standard stats approaches to our results, to see what we got.
			
#.
	The Stats Approaches
		Mean, Median, and Mode; 
			Why display one when you can calculate and print them all? Especially when they agree with each other, further supporting the quality of 
			the test!