function speakMuda() {
    if (!("speechSynthesis" in window)) {
        return;
    }
    var utterance = new SpeechSynthesisUtterance("BELIN ALORA SEI SCEMO");
    utterance.lang = "it-IT";
    utterance.rate = 0.9;
    utterance.pitch = 0.6;
    utterance.volume = 1;
    window.speechSynthesis.cancel();
    window.speechSynthesis.speak(utterance);
}