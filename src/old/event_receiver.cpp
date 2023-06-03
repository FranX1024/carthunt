bool EventReceiver::OnEvent(const SEvent &event) {
  if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
    keydown[event.KeyInput.Key] = event.KeyInput.PressedDown;
  }
  return false;
}
