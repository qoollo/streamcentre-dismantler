class SegmenterMock:
    running_streams = set()

    def __init__(self):
        self.running_streams.clear()

    def start_recording(self, stream_url, stream_id):
        self.running_streams.add(stream_id)

    def stop_recording(self, stream_id):
        if stream_id not in self.running_streams:
            raise ValueError("no stream with given id")

    def get_state(self, stream_id):
        if stream_id in self.running_streams:
            return RecorderStateMock(is_running=True, is_connected=True)
        else:
            raise ValueError("stream not started")


class RecorderStateMock:
    is_running = False
    is_connected = False
    start_time = False
    message = ''

    def __init__(self, is_running, is_connected, start_time=0, message=''):
        self.is_running = is_running
        self.is_connected = is_connected
        self.start_time = start_time
        self.message = message
