#ifndef DOOGIE_CEF_HANDLER_H_
#define DOOGIE_CEF_HANDLER_H_

#include <QtWidgets>
#include <vector>

#include "cef/cef.h"

namespace doogie {

class CefHandler :
    public QObject,
    public CefClient,
    public CefContextMenuHandler,
    public CefDisplayHandler,
    public CefFindHandler,
    public CefFocusHandler,
    public CefJSDialogHandler,
    public CefKeyboardHandler,
    public CefLifeSpanHandler,
    public CefLoadHandler,
    public CefRequestHandler {
  Q_OBJECT

 public:
  // Matches CEF's numbering, don't change
  enum WindowOpenType {
    OpenTypeUnknown,
    OpenTypeCurrentTab,
    OpenTypeSingletonTab,
    OpenTypeNewForegroundTab,
    OpenTypeNewBackgroundTab,
    OpenTypeNewPopup,
    OpenTypeNewWindow,
    OpenTypeSaveToDisk,
    OpenTypeOffTheRecord,
    OpenTypeIgnoreAction
  };
  Q_ENUM(WindowOpenType)

  CefHandler();

  CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override {
    return this;
  }

  CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
    return this;
  }

  CefRefPtr<CefFindHandler> GetFindHandler() override {
    return this;
  }

  CefRefPtr<CefFocusHandler> GetFocusHandler() override {
    return this;
  }

  CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override {
    return this;
  }

  CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override {
    return this;
  }

  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
    return this;
  }

  CefRefPtr<CefLoadHandler> GetLoadHandler() override {
    return this;
  }

  CefRefPtr<CefRequestHandler> GetRequestHandler() override {
    return this;
  }

  // Context menu handler overrides...
  void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefContextMenuParams> params,
                           CefRefPtr<CefMenuModel> model) override;

  bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            CefRefPtr<CefContextMenuParams> params,
                            int command_id,
                            EventFlags event_flags) override;

  // Display handler overrides...
  void OnAddressChange(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefFrame> frame,
                       const CefString& url) override;
  void OnTitleChange(CefRefPtr<CefBrowser> browser,
                     const CefString& title) override;
  void OnStatusMessage(CefRefPtr<CefBrowser> browser,
                       const CefString& value) override;
  void OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
                          const std::vector<CefString>& icon_urls) override;

  // Find handler overrides...
  void OnFindResult(CefRefPtr<CefBrowser> browser,
                    int identifier,
                    int count,
                    const CefRect& selection_rect,
                    int active_match_ordinal,
                    bool final_update) override;

  // Focus handler overrides...
  void OnGotFocus(CefRefPtr<CefBrowser> browser) override;
  bool OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source) override;

  // JS dialog handler overrides...
  typedef std::function<void(const QString&,
                             JSDialogType,
                             const QString&,
                             const QString&,
                             CefRefPtr<CefJSDialogCallback>,
                             bool&)> JsDialogCallback;
  bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                  const CefString& origin_url,
                  JSDialogType dialog_type,
                  const CefString& message_text,
                  const CefString& default_prompt_text,
                  CefRefPtr<CefJSDialogCallback> callback,
                  bool& suppress_message) override;
  void SetJsDialogCallback(JsDialogCallback callback);
  bool OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
                            const CefString& message_text,
                            bool is_reload,
                            CefRefPtr<CefJSDialogCallback> callback) override;

  // Key handler overrides...
  bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
                  const CefKeyEvent& event,
                  CefEventHandle os_event) override;

  // Life span handler overrides...
  bool OnBeforePopup(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      const CefString& target_url,
      const CefString& target_frame_name,
      CefLifeSpanHandler::WindowOpenDisposition target_disposition,
      bool user_gesture,
      const CefPopupFeatures& popupFeatures,
      CefWindowInfo& windowInfo,
      CefRefPtr<CefClient>& client,
      CefBrowserSettings& settings,  // NOLINT(runtime/references)
      bool* no_javascript_access) override;
  bool DoClose(CefRefPtr<CefBrowser> browser) override;
  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

  // Load handler overrides...
  void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                            bool is_loading,
                            bool can_go_back,
                            bool can_go_forward) override;
  void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                 CefRefPtr<CefFrame> frame,
                 int httpStatusCode) override;
  void OnLoadStart(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefFrame> frame,
                   TransitionType transition_type);

  // Request handler overrides...
  bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                      CefRefPtr<CefFrame> frame,
                      CefRefPtr<CefRequest> request,
                      bool is_redirect) override;
  bool OnOpenURLFromTab(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      const CefString& target_url,
      CefRequestHandler::WindowOpenDisposition target_disposition,
      bool user_gesture) override;

 signals:
  void PreContextMenu(CefRefPtr<CefContextMenuParams> params,
                      CefRefPtr<CefMenuModel> model);
  void ContextMenuCommand(CefRefPtr<CefContextMenuParams> params,
                          int command_id,
                          EventFlags event_flags);
  void UrlChanged(const QString& url);
  void TitleChanged(const QString& title);
  void StatusChanged(const QString& status);
  // Empty if no URL
  void FaviconUrlChanged(const QString& url);
  void FindResult(int identifier,
                  int count,
                  const CefRect& selection_rect,
                  int active_match_ordinal,
                  bool final_update);
  void FocusObtained();
  void ShowBeforeUnloadDialog(const QString& message_text,
                              bool is_reload,
                              CefRefPtr<CefJSDialogCallback> callback);
  void KeyEvent(const CefKeyEvent& event, CefEventHandle os_event);
  void Closed();
  void AfterCreated(CefRefPtr<CefBrowser> browser);
  void LoadStateChanged(bool is_loading,
                        bool can_go_back,
                        bool can_go_forward);
  void LoadEnd(CefRefPtr<CefFrame> frame,
               int httpStatusCode);
  void LoadStart(CefLoadHandler::TransitionType transition_type);
  void PageOpen(WindowOpenType type, const QString& url, bool user_gesture);
  void BrowserLog(const QString& str);

 private:
  bool popup_as_page_open_ = true;
  JsDialogCallback js_dialog_callback_;

  IMPLEMENT_REFCOUNTING(CefHandler)
};

}  // namespace doogie

#endif  // DOOGIE_CEF_HANDLER_H_
