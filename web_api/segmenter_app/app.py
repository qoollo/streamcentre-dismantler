from flask import Flask, jsonify, request
import sys
from libsegmenter_py import createStreamsManager

app = Flask(__name__, instance_relative_config=True)
app.config.from_object('segmenter_app.default_settings')
app.config.from_pyfile('application.cfg', silent=True)

segmenter = None


def init_app(app, seg=None):
    global segmenter

    if seg is None:
        segmenter = createStreamsManager(app.config['SEGMENTER_CONFIG_FILE_NAME'])
    else:
        segmenter = seg


@app.route('/index')
def index():
    return jsonify(result="hello..")


@app.route('/start_rec/<int:stream_id>', methods=['GET', 'POST'])
def start_rec(stream_id):
    try:
        url = request.args.get('url').encode('ascii', 'ignore')
        segmenter.start_recording(url, stream_id)
        return jsonify(result="ok")
    except ValueError:
        return jsonify(error_message="stream already started"), 500


@app.route('/stop_rec/<int:stream_id>', methods=['GET', 'POST'])
def stop_rec(stream_id):
    try:
        segmenter.stop_recording(stream_id)
        return jsonify(result="ok")
    except ValueError:
        return jsonify(error_message="stream not started"), 404


@app.route('/get_state/<int:stream_id>', methods=['GET', 'POST'])
def get_state(stream_id):
    try:
        state = segmenter.get_state(stream_id)
        return jsonify(
            is_running=state.is_running,
            is_connected=state.is_connected,
            status_message=state.message)

    except ValueError:
        return jsonify(error_message="stream not started"), 404


@app.route('/test_saver', methods=['POST'])
def test_saver():
    stream_id = int(request.form.get('stream_id'))
    start_time = float(request.form.get('start_time'))
    stop_time = float(request.form.get('stop_time'))
    container_format = request.form.get('container_format')
    data = request.files['data']
    file_name = \
        "test_resources/test_trash/{}_{:0.1f}-{:0.1f}.{}" \
            .format(stream_id, start_time, stop_time, container_format)
    data.save(file_name)
    return jsonify(result="ok"), 200
