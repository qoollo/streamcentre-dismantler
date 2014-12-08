import unittest
import json
from segmenter_app.app import app, init_app
from tests.segmenter_mock import SegmenterMock


class SegmenterAppTestCase(unittest.TestCase):
    def setUp(self):
        app.config.from_object('tests.test_settings')
        init_app(app, SegmenterMock())
        self.app = app.test_client()

    def test_index(self):
        ans = self.app.get('/index')
        self.assertEqual(ans.status_code, 200)
        json_ans = json.loads(ans.data)
        self.assertEqual(json_ans['result'], 'hello..')

    def test_rec(self):
        start_ans = self.app.post('/start_rec/1?url={0}'.format(app.config['TEST_STREAM_URL']))
        self.assertEqual(start_ans.status_code, 200)

    def test_state_started(self):
        start_ans = self.app.post('/start_rec/1?url={0}'.format(app.config['TEST_VIDEO_URL']))
        assert start_ans.status_code == 200
        state_ans = self.app.post('/get_state/1')
        self.assertEqual(state_ans.status_code, 200)

    def test_state_not_started(self):
        state_ans = self.app.post('/get_state/123')
        self.assertEqual(state_ans.status_code, 404)

    def test_stop_started(self):
        start_ans = self.app.post('/start_rec/34?url={0}'.format(app.config['TEST_VIDEO_URL']))
        assert start_ans.status_code == 200
        stop_ans = self.app.post('/stop_rec/34')
        self.assertEqual(stop_ans.status_code, 200)

    def test_stop_not_started(self):
        stop_ans = self.app.post('/stop_rec/34')
        self.assertEqual(stop_ans.status_code, 404)

if __name__ == '__main__':
    unittest.main()
