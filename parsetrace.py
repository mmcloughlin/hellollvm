import sys
import logging
import collections

import cxxfilt

Event = collections.namedtuple('Event', 'tipe function timestamp')


def read_trace_lines(lines):
    """
    Extract trace events from output.
    """
    for line in lines:
        parts = line.split()
        if len(parts) != 4 or parts[0] != 'TRACE:':
            continue
        yield Event(tipe=parts[1], function=parts[2], timestamp=float(parts[3]))


def verify_trace(events):
    """
    Confirm trace events are correctly nested.
    """
    stack = []
    for event in events:
        if event.tipe == 'enter':
            stack.append(event)
        elif event.tipe == 'leave':
            assert len(stack) > 0, 'observed leave event with empty stack'
            cur = stack.pop()
            assert cur.function == event.function, 'leave event not paired with enter'
            assert cur.timestamp <= event.timestamp, 'timestamps out-of-order'
        else:
            assert False, 'unknown event type "{}"'.format(event.tipe)
    assert len(stack) == 0, 'stack should be empty at the end of the trace'


def extract_function_durations(events):
    """
    Extract function durations from a trace.
    """
    durations = {}
    stack = []
    for event in events:
        if event.tipe == 'enter':
            stack.append(event)
        elif event.tipe == 'leave':
            enter = stack.pop()
            durations.setdefault(enter.function, []).append(
                event.timestamp - enter.timestamp)
        else:
            assert False
    return durations


def mean(x):
    return sum(x) / float(len(x))


def format_time(secs):
    t = secs
    units = ['s', 'ms', 'us', 'ns']
    for unit in units:
        if t >= 1.0:
            break
        t *= 1e3
    return '{:f}{:s}'.format(t, unit)


def main():
    input_filename = sys.argv[1]
    with open(input_filename) as f:
        events = list(read_trace_lines(f))

    verify_trace(events)

    profile = extract_function_durations(events)
    for function, durations in profile.items():
        avg = mean(durations)
        print '{fn}\n\tavg = {avg}\n'.format(
            fn=cxxfilt.demangle(function),
            avg=format_time(avg),
        )


if __name__ == '__main__':
    main()
