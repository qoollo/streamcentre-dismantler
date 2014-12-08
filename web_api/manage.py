from flask.ext.script import Manager
from segmenter_app.app import app, init_app
from flask import Flask

# configure your app

init_app(app)
manager = Manager(app)

if __name__ == "__main__":
    manager.run()
