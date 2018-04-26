CSCI 340 Final Project
Jack Fraser & Anthony Morrell
4.25.2018
Dr Leclerc

1. 
	How big is a cache block?
		Experimentally, we found that 128 was an extremely reliable value, tested by finding the value at which increasing to the next power of
		2 had the greatest % increase in time per request. While this value was only a tiny bit larger than its neighbors, the result was
		unique among all our tests in that it always reached the same exact value, for all machines, and under all circumstances. So either we
		did something very very right, or something very very wrong.
		
		8) How we tested for it:
			This one went through many revisions before reaching the final iteration. In the end, our solution was based on the idea that each
			block would have to be a power of 2 size. Therefore, only the first 9 or so powers of 2 would have to be tested, as past that reaches
			some truly ridiculous numbers. This gave us a finite number of tests (9), with which to generate a nice big array in, jump through the
			nth	items in that array till its end, and find the average times to do so for each size. Oddly, while we expected the average get times
			to plateau (in the case of every call being outside of cache), they instead tended to increase by very close to a factor of 2, in step
			with the increase in size of the array. However, it was that variance that allowed us to interpret an answer. While the times increased
			by close to 2 for every step length doubling, the variance was calculatable, and consistent. It showed that the 256 bit step had the 
			largest increase by a small but meaningful margin. This read to us to mean that 256 was large enough to consistently break the paging, 
			and therefore that the previous power, 128, must be the correct value. Our method outputs the value automatically selected as the size 
			of the array smaller than the one with the largest increase... and we have every confidence that your machine, like all of ours, will 
			find the same result.
		
2. 
	How big is the cache?
		Finding the three levels of cache posed a significantly harder question. In fact, even with iterations running until the very last few
		hours of the many times extended deadline, we still do not have a method that reliably finds the correct values (as found in hardware
		specs). However, correllating inversely to the inaccuracy of our test results is the complexity and quality of our tests; 
		
		7) How we tested for it:
			By far the most complex, complicated, and least well answered or understood... cache size is an elusive thing to test for, and so
			easily and concretely looked up that the lack of finding results to support our pre-existing knowledge was tantamount to torture.
			While one version of the calculation would semi-consistenly find a cache size of 0.1 Megabytes, which is not an unbelievable number
			(being within an order of most real answers for most cache layers, on most machines)... it would also occasionally return 8M, 10M,
			or other elusive, questionable results. These variances are largely due to the inability of our coding minds to move the timing
			mechanism outside of the innermost for loop; meaning that any test, over any number of tests, and any number of permutations, would
			always be carrying significant error forward. In attempts to normalize the results, the method was rewritten multiple times, first 
			to refine the scope of sizes tested for to allow for more tests; then by reducing the test count to allow for greater precision in
			the estimated numbers; then by undoing that because the high precision numbers were just more wrong, and it took too long. We even
			went into calculating the percent difference over previous values, looking for patterns there: Is there a consistent single high
			value or percentage that lines up with a potential cache layer size? What about a low one followed by a high one? What about a
			grouping of similar values indicating they all fit within the same cache layer? Sadly, while any one test would return a number of
			promising results in all of these potential directions (sometimes even hitting the exact known cache size values), any subsequent
			test would be just as like as not to miss by as wide a margin as was possible in the allowed range of resulting values. In the end,
			cache size is still a mystery; a kraken, laughing leagues below in the depths as we try to swim down to grab it, or even catch a 
			glimpse.
			
		
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
			
#
	The Print Statements
		If you wanna have some fun with it, try uncommenting some of the print statements! Some of them (especially in the cache size method), do some 
		REALLY cool things!