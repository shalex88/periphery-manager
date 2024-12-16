# pip install protobuf
import sys
import socket

# Add the generated directory to the Python path
sys.path.append('generated')
import api_pb2  # Import the generated protobuf module

def tcp_client(host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        print(f"[TCP Client] Connected to {host}:{port}")

        try:
            while True:
                message = input("[TCP Client] Enter message: ")

                # Create and serialize CommandRequest
                command_request = api_pb2.CommandRequest()
                command_request.action = message
                serialized_message = command_request.SerializeToString()

                s.sendall(serialized_message)

                print(f'[TCP Client] Sent ({len(serialized_message)} bytes): "{message}"')

                # Wait to receive echo back (optional, depends on your use case)
                data = s.recv(1024)
                if data:
                    data_list = list(data)
                    print(f'[TCP Client] Received ({len(data)} bytes): "{data.decode()}" [{" ".join(map(str, data_list))}]')

                    # Deserialize the response if it's a CommandRequest
                    # response = api_pb2.CommandRequest()
                    # response.ParseFromString(data)
                    # print(f'[TCP Client] Received ({len(data)} bytes): "{response.action}"')

        except KeyboardInterrupt:
            print("\n[TCP Client] Shutting down")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        HOST = sys.argv[1]
    else:
        HOST = socket.gethostbyname(socket.gethostname())

    if len(sys.argv) > 2:
        PORT = int(sys.argv[2])
    else:
        PORT = 12345

    tcp_client(HOST, PORT)