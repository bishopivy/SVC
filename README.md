# Audio Plugin for so-vits-svc

Use the so-vits-svc singing audio vocal transformer in an audio recording software instead of using python notebooks. <br>
Note this works for Mac Only.

## Required Installations
- [JUCE](https://juce.com/download/)
- [XCode](https://apps.apple.com/us/app/xcode/id497799835?mt=12)
- so-vits-svc: `pip3.9 install -U git+https://github.com/34j/so-vits-svc-fork.git`
    - Note: so-vits-svc must be installed for python 3.9 in order to function in this plugin.

## File Setup 
- create the directory `~/Music/code_stuff/so-vits-svc-fork/notebooks/logs/44k/` and move `config.json` there.
- download the file `bishop2.pth` from Google Drive (download link sent to HAI instructors).
- create the directory `~/Music/code_stuff/so-vits-svc-fork/models/` and move `bishop2.pth` there.

## Building 
- open `SVC.jucer` using the Projucer app that comes with JUCE
- In Projucer, click File->Save Project and Open in IDE...
    - This should open XCode 
- In XCode click Product->Scheme->Choose Scheme
    - Select SVC - AU
- In XCode click Product->Scheme->Edit Scheme
    - Make Sure Build Configuration is set to `Debug`
    - Under executable, click `Other`, then search for AudioPluginHost and select.
- Run in XCode (Product->Run, command R).

## Running 
- Perform the steps in Building, which will open AudioPluginHost.
If you haven't done initialized AudioPluginHost before:
- Options->Edit the List of Avaliable Plugins.
- Click `Options...` in the bottom left corner, then `Scan for new or updated AudioUnit plug-ins`.
- Exit out of this menu.
- Click Options->Plug-in Menu Type->List Plugins by Manufacturer.
- File->Open...
    - Select `example.filtergraph`.
- Double Click `AUAudioFilePlayer (AudioUnit)`.
    - Drag the file you want to turn into Bishop's voice into the list of Audio Files.
- Save (command S)
- Stop the process in XCode and start again.<br>
Test the plugin:
-  Double Click `AUAudioFilePlayer (AudioUnit)`. If its UI isn't already open 
- Double Click `SVC (AudioUnit)`. If its UI isn't already open 
- Check `Transfer` in the SVC plugin
- Play your audio file 
- uncheck `Transfer`
- Wait a few seconds to hear the converted version. 