package com.huaming.deskplate;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Color;
import android.net.http.SslError;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.WindowManager;
import android.webkit.SslErrorHandler;
import android.webkit.WebResourceError;
import android.webkit.WebResourceRequest;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MainActivity extends Activity {
    private WebView webView;
    private View errorView;
    private String deskplateUrl;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        deskplateUrl = getString(R.string.deskplate_url);
        enableFullscreen();
        setContentView(createContentView());
        if (initializeWebView()) {
            loadDeskplate();
        } else {
            showConnectionError();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        enableFullscreen();
        if (webView != null) {
            try {
                webView.onResume();
            } catch (RuntimeException ignored) {
                showConnectionError();
            }
        }
    }

    @Override
    protected void onPause() {
        if (webView != null) {
            try {
                webView.onPause();
            } catch (RuntimeException ignored) {
                showConnectionError();
            }
        }
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        if (webView != null) {
            try {
                webView.destroy();
            } catch (RuntimeException ignored) {
                // Avoid crashing during Activity teardown if the platform WebView is unhealthy.
            }
        }
        super.onDestroy();
    }

    @Override
    public void onBackPressed() {
        if (webView != null && webView.canGoBack()) {
            webView.goBack();
            return;
        }
        super.onBackPressed();
    }

    private View createContentView() {
        FrameLayout root = new FrameLayout(this);
        root.setLayoutParams(new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
        ));

        errorView = createErrorView();
        errorView.setVisibility(View.GONE);
        root.addView(errorView);
        return root;
    }

    private View createErrorView() {
        LinearLayout container = new LinearLayout(this);
        container.setOrientation(LinearLayout.VERTICAL);
        container.setGravity(Gravity.CENTER);
        container.setBackgroundColor(getColor(R.color.error_background));
        int horizontalPadding = dp(28);
        container.setPadding(horizontalPadding, 0, horizontalPadding, 0);
        container.setLayoutParams(new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
        ));

        TextView title = new TextView(this);
        title.setText(R.string.connection_error_title);
        title.setTextColor(getColor(R.color.error_text));
        title.setTextSize(24);
        title.setGravity(Gravity.CENTER);
        title.setTypeface(title.getTypeface(), android.graphics.Typeface.BOLD);
        container.addView(title, new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));

        TextView message = new TextView(this);
        message.setText(R.string.connection_error_message);
        message.setTextColor(getColor(R.color.error_secondary_text));
        message.setTextSize(17);
        message.setGravity(Gravity.CENTER);
        message.setPadding(0, dp(14), 0, dp(28));
        container.addView(message, new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));

        Button retryButton = new Button(this);
        retryButton.setText(R.string.retry);
        retryButton.setTextColor(Color.WHITE);
        retryButton.setTextSize(16);
        retryButton.setAllCaps(false);
        retryButton.setBackgroundResource(R.drawable.retry_button_background);
        retryButton.setOnClickListener(v -> loadDeskplate());
        container.addView(retryButton, new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));

        return container;
    }

    @SuppressLint("SetJavaScriptEnabled")
    private boolean initializeWebView() {
        WebView candidate = null;
        try {
            candidate = new WebView(this);
            candidate.setLayoutParams(new FrameLayout.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT,
                    ViewGroup.LayoutParams.MATCH_PARENT
            ));

            webView = candidate;
            configureWebView();

            ViewGroup root = (ViewGroup) errorView.getParent();
            root.addView(webView, 0);
            return true;
        } catch (RuntimeException ignored) {
            if (candidate != null) {
                try {
                    candidate.destroy();
                } catch (RuntimeException destroyIgnored) {
                    // Keep the native error screen visible instead of crashing.
                }
            }
            webView = null;
            return false;
        }
    }

    @SuppressLint("SetJavaScriptEnabled")
    private void configureWebView() {
        WebSettings settings = webView.getSettings();
        settings.setJavaScriptEnabled(true);
        settings.setDomStorageEnabled(true);
        settings.setLoadWithOverviewMode(true);
        settings.setUseWideViewPort(true);
        settings.setCacheMode(WebSettings.LOAD_NO_CACHE);

        webView.setWebViewClient(new WebViewClient() {
            @Override
            public void onPageFinished(WebView view, String url) {
                if (deskplateUrl.equals(url) || url.startsWith(deskplateUrl + "/")) {
                    showWebView();
                }
            }

            @Override
            public void onReceivedError(WebView view, WebResourceRequest request, WebResourceError error) {
                if (request.isForMainFrame()) {
                    showConnectionError();
                }
            }

            @Override
            public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
                showConnectionError();
            }

            @Override
            public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error) {
                handler.cancel();
                showConnectionError();
            }
        });
    }

    private void loadDeskplate() {
        if (webView == null && !initializeWebView()) {
            showConnectionError();
            return;
        }

        try {
            errorView.setVisibility(View.GONE);
            webView.setVisibility(View.VISIBLE);
            webView.loadUrl(deskplateUrl);
        } catch (RuntimeException ignored) {
            showConnectionError();
        }
    }

    private void showWebView() {
        if (webView != null) {
            webView.setVisibility(View.VISIBLE);
        }
        errorView.setVisibility(View.GONE);
    }

    private void showConnectionError() {
        if (webView != null) {
            try {
                webView.stopLoading();
            } catch (RuntimeException ignored) {
                // Keep the native error screen visible instead of crashing.
            }
            webView.setVisibility(View.GONE);
        }
        errorView.setVisibility(View.VISIBLE);
    }

    private void enableFullscreen() {
        Window window = getWindow();
        window.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.R) {
            window.setDecorFitsSystemWindows(false);
            WindowInsetsController controller = window.getInsetsController();
            if (controller != null) {
                controller.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
                controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        } else {
            window.getDecorView().setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            );
        }
    }

    private int dp(int value) {
        return Math.round(value * getResources().getDisplayMetrics().density);
    }
}
