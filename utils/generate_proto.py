import os
import subprocess

def generate_protobuf():
    proto_dir = '../proto/internal_api'
    proto_file = 'api.proto'
    output_dir = 'generated'

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    protoc_command = [
        'protoc',
        f'--proto_path={proto_dir}',
        f'--python_out={output_dir}',
        os.path.join(proto_dir, proto_file)
    ]

    result = subprocess.run(protoc_command, capture_output=True, text=True)

    if result.returncode != 0:
        print(f"Error generating protobuf: {result.stderr}")
    else:
        print("Protobuf generated successfully")

if __name__ == "__main__":
    generate_protobuf()