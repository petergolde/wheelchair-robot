using System;

using AVFoundation;
using Foundation;

namespace Thingamabot
{
    public static class SoundPlayer
    {
        public static void PlayWavFile(string filename)
        {
            var mediaFile = NSUrl.FromFilename(filename);
            var audioPlayer = AVAudioPlayer.FromUrl(mediaFile);
            audioPlayer.Play();
        }
    }
}

