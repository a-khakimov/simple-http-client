from http.server import HTTPServer, BaseHTTPRequestHandler
import json

HOST_ADDRESS = ""
HOST_PORT = 8000


class FakeDatabase:
    table = {}

    def update(self, id, method="", data=""):
        try:
            d = self.table[id]
            d[method] = data
        except KeyError:
            self.table[id] = {method: data}

    def getAll(self):
        return self.table

db = FakeDatabase

class RequestHandler(BaseHTTPRequestHandler):

    def send_response(self, code, message=None):
        """ override to customize header """
        self.log_request(code)
        self.send_response_only(code)
        self.send_header('Server', 'python3 http.server Development Server')
        self.send_header('Date', self.date_time_string())
        self.end_headers()

    def do_GET(self):
        info = db.getAll(db)
        jsonInfo = json.dumps(info)
        print(info)

        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        self.wfile.write(jsonInfo.encode(encoding='utf_8'))

    def do_POST(self):
        if self.path != "/api/json/v2":
            self.send_response(404)
            return 

        content_length = int(self.headers['Content-Length'])
        content = self.rfile.read(content_length)
        try:
            jsonContent = json.loads(content)
            id = int(jsonContent["id"])
            method = jsonContent["method"]
            params = jsonContent["params"]
            db.update(db, id, method, params)
            self.send_response(201)
        except (KeyError, json.decoder.JSONDecodeError):
            self.send_response(404, "JSON Error")


def run(server_class=HTTPServer, handler_class=BaseHTTPRequestHandler):
    """ follows example shown on docs.python.org """
    server_address = (HOST_ADDRESS, HOST_PORT)
    httpd = server_class(server_address, handler_class)
    httpd.serve_forever()


if __name__ == '__main__':
    run(handler_class=RequestHandler)
