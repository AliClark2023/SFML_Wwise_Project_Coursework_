# SFML Wwise Project Notes

This project provides a basic Wwise implementation integrated into an SFML
project. The audio code is not at all tied to SFML though; the same approach can
be used with any C++ project/framework (e.g. SDL, skateboard, etc.).

The integration follows the official Audiokinetic guidance at this link:
https://www.audiokinetic.com/en/public-library/2025.1.5_9095/?source=SDK&id=workingwithsdks.html

There were a few idiosyncracies to the integration that were not listed in the
official docs, you can compare the code in `WwiseWrapper` to the official docs
to see what's different.

Note that `WwiseWrapper` provides just the barebones of a Wwise integration. You
will likely need to extend it to make it work for your purposes.

Note also that you will need Wwise installed (including the SDK), and may need
to update the header and library search paths in the project settings to point
to your Wwise SDK installation.

The project is built for **SFML 3.0.2** and **Wwise 2024.1.8.8898**.

## Visual Studio Project Settings
Setting up your Visual Studio project to work with Wwise is similar to using any
external library: you will need to set the **Additional Include Directories** to
point to your Wwise SDK `include` folder, the **Additional Libraries
Directories** to point to the appropriate SDK `lib` folder (note there are
separate libraries for Debug and Release versions), and you will need to include
the following libraries:

|Library				 |Debug Only|
|------------------------|----------|
|AkSoundEngine.lib		 |			|
|AkMemoryMgr.lib		 |			|
|AkStreamMgr.lib		 |			|
|AkSpatialAudio.lib		 |			|
|AkMusicEngine.lib		 |			|
|CommunicationCentral.lib|Yes		|
|wsock32.lib			 |Yes		|

You will also want to add `AK_OPTIMIZED` to the **Preprocessor Definitions** for
the release build.

## SoundEngine Folder
By default, Wwise expects you to implement a lot of file I/O and memory handling
yourself. Audiokinetic provide a reference implementation however. These files
are included in the **SoundEngine** folder.