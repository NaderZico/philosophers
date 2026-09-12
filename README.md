# Philosophers

A solution to the classic dining philosophers problem in C using POSIX threads and mutexes. Built at 42 Abu Dhabi.

## What it does
- Creates one thread per philosopher sharing a table of forks
- Uses mutexes to protect forks and a monitor to detect death or completion
- Tracks time in milliseconds to decide when a philosopher eats, sleeps, or must stop

## Build and run
```bash
make
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

Example:
```bash
./philo 5 800 200 200
```

```bash
make clean / make fclean / make re
```

## What I learned
- Thread synchronization and avoiding race conditions
- Preventing deadlocks while sharing limited resources
- Precise timing and clean thread shutdown
