class EventReceiver : public IEventReceiver {
public:
  virtual bool OnEvent(const SEvent &event);
} event_receiver;

bool keydown[KEY_KEY_CODES_COUNT];
