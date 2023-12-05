import os
import subprocess

model_path = "/Users/bishopcrowley/Music/code_stuff/so-vits-svc-fork/models/jack1.pth"
config_path = "/Users/bishopcrowley/Music/code_stuff/so-vits-svc-fork/notebooks/logs/44k/config.json"
file_path = "/Users/bishopcrowley/Music/svc_test.wav"
command = "svc infer " + file_path + " -m " + model_path + " -c " + config_path + " -d cpu -na"

log = '/Users/bishopcrowley/Music/python.txt'
with open(log, 'w') as file:
    # Write content to the file
    file.write('about to run the command!!\n')
# os.system(command)
result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
with open(log, "a") as file:
    file.write(result.stdout)

with open(log, 'a') as file:
    # Write content to the file
    file.write('i did it!!\n')