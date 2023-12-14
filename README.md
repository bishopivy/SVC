# Audio Plugin for so-vits-svc

Use the so-vits-svc singing audio vocal transformer in an audio recording software instead of using python notebooks. <br>
Note this works for Mac Only.

## Required Installations
- JUCE: [download](https://juce.com/download/)<br>
- XCODE: [download](https://apps.apple.com/us/app/xcode/id497799835?mt=12)
- so-vits-svc: `pip3.9 install -U git+https://github.com/34j/so-vits-svc-fork.git`
- - Note: so-vits-svc must be installed for python 3.9 in order to function in this plugin.

## File Setup 
- create the directory `~/Music/code_stuff/so-vits-svc-fork/notebooks/logs/44k/` and move `config.json` there.
- download the file `bishop2.pth` from Google Drive (download link sent to HAI instructors).
- create the directory `~/Music/code_stuff/so-vits-svc-fork/models/` and move `bishop2.pth` there.