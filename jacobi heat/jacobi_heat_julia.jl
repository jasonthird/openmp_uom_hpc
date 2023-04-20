using .Threads

const maxsize = 500
const iterations = 1000000
const row = 51
const col = 51
const start = 100
const accuracy = 27

function main()

  # initialize tables and data
  table1 = zeros(maxsize, maxsize)
  table2 = zeros(maxsize, maxsize)
  diff = 0.0
  
  #measure time
  t1 = time()
  # repeat for each iteration
  for k = 1:iterations
    # create a heat source
    table1[row, col] = start
    
    # difference initialization
    diff = 0.0
    
    diffarray = zeros(Threads.nthreads())

    # perform the calculations
    Threads.@threads for i = 2:maxsize-1
      tempdiff = 0.0
      @simd for j = 2:maxsize-1
        @inbounds table2[i, j] = (table1[i-1, j] + table1[i+1, j] + table1[i, j-1] + table1[i, j+1]) / 4
        @inbounds tempdiff += (table2[i, j] - table1[i, j])^2
      end
      # atomic_add!(diff, tempdiff)
      diffarray[Threads.threadid()] += tempdiff
    end
    
    for i = 1:Threads.nthreads()
      diff += diffarray[i]
    end
    # print difference and check convergence
    diff = sqrt(diff)
    #println("diff = $diff")
    
    if (diff < accuracy)
      println("\n\nConvergence in $k iterations\n\n")
      println("diff = $diff")
      break
    end
    
    # copy new table to old table
    for i = 1:maxsize, j = 1:maxsize
      table1[i, j] = table2[i, j]
    end
  end
  t2 = time()
  
  # print time
  println("Time taken: $(t2-t1)")


  return 0
end

main()
