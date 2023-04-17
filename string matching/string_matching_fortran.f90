program string_matching
  
  use omp_lib

  implicit none
  

  integer, parameter :: max_file_name_len = 256
  integer, parameter :: max_pattern_len = 256

  character(len=max_file_name_len) :: file_name
  character(len=:), allocatable :: pattern
  character(len=:), allocatable :: buffer
  integer :: file_size, pattern_size, match_size, total_matches
  integer, allocatable :: match(:)
  integer :: i, j, k, unit_number
  logical :: file_exists
  double precision :: start_time, end_time

  ! Check command line arguments
  if (command_argument_count() /= 2) then
    print *, "Usage: string_matching <file_name> <pattern>"
    stop 1
  end if

  allocate(character(len=max_pattern_len) :: pattern)

  ! Get file name and pattern from command line arguments
  call get_command_argument(1, file_name)
  call get_command_argument(2, pattern)

  ! Check if file exists
  inquire(file=file_name, exist=file_exists)
  if (.not. file_exists) then
    print *, "File error"
    stop 2
  end if

  ! Get file size
  inquire(file=file_name, size=file_size)

  ! Allocate buffer to contain the file
  allocate(character(len=file_size) :: buffer)

  ! Read file into buffer
  open(newunit=unit_number, file=file_name, access="stream", &
       action="read", status="old")
  read(unit_number) buffer
  close(unit_number)

  ! Get pattern size and match size
  pattern_size = len_trim(pattern)
  match_size = file_size - pattern_size + 1

  ! Allocate match array
  allocate(match(match_size))

  ! Initialize match array to zero
  match = 0
  total_matches = 0

  ! Brute force string matching
  start_time = omp_get_wtime()
  !$omp parallel do private(i,j,k) firstprivate(pattern_size,match_size) shared(match,buffer,pattern) reduction(+:total_matches)
    do j = 1, match_size
      i=1
      do while(i <= pattern_size .and. buffer(j+i-1:j+i-1) == pattern(i:i))
        i = i + 1
      end do

      if (i > pattern_size) then
          match(j) = 1
          total_matches = total_matches + 1
      end if
    end do
  !$omp end parallel do

  end_time = omp_get_wtime()

  ! Print results
  print *, "Time taken:", end_time - start_time, "seconds for", file_size, "bytes"
  print *, "Total matches =", total_matches , "for pattern ", pattern(1:pattern_size)

  ! Clean up
  deallocate(buffer)
  deallocate(match)

end program string_matching
