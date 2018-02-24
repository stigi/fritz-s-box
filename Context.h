#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <SoftwareSerial.h>
#include "DFMiniMp3.h"

class Mp3Notify;

typedef struct {
  DFMiniMp3<SoftwareSerial, Mp3Notify> *mp3;
  bool isPlaying;
  bool isPaused;
  int currentFolder; // increment + 1 for usage in mp3 api
  int currentTrack;  // increment + 1 for usage in mp3 api
  int tracksInFolders[13]; // ignoring index 0
} Context;

void togglePlayPause(Context *context) {
  DFMiniMp3<SoftwareSerial, Mp3Notify> *mp3 = context->mp3;

  if (context->isPlaying) {
    if (context->isPaused) {
      Serial.print(F("Play"));
      mp3->start();
      context->isPaused = false;
    } else {
      Serial.print(F("Pause"));
      mp3->pause();
      context->isPaused = true;
    }
  }
}

void playFolderAndTrack(int folder, int track, Context *context) {
  DFMiniMp3<SoftwareSerial, Mp3Notify> *mp3 = context->mp3;
  
  if (track <= context->tracksInFolders[folder]) {
    // folders and tracks are 1-indexed
    //play specific mp3 in SD:/01/001.mp3; Folder Name(1~99); File Name(1~255)
    Serial.print(F("Playing track "));
    Serial.print(track);
    Serial.print(F(" in folder "));
    Serial.println(folder);
    mp3->playFolderTrack(folder, track);
    context->isPlaying = true;
    context->currentFolder = folder;
    context->currentTrack = track;
  } else {
    Serial.print(F("Could not play track "));
    Serial.print(track);
    Serial.print(F(" in folder "));
    Serial.println(folder);
    Serial.println(F("Stopping"));
    mp3->stop();
    context->isPlaying = false;
    context->currentFolder = 0;
    context->currentTrack = 0;
  }
}

void playNextTrack(Context *context) {
  int currentFolder = context->currentFolder;
  int currentTrack = context->currentTrack;
  playFolderAndTrack(currentFolder, currentTrack+1, context);
}

void handlePlayFinished(Context *context) {
  playNextTrack(context);
}

#endif
