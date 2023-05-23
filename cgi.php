<?php
// Set the cookie
$cookieName = "example_cookie";
$cookieValue = "Hello, cookies!";
$cookieExpiration = time() + (86400 * 30); // 30 days

setcookie($cookieName, $cookieValue, $cookieExpiration);

// Retrieve the cookie
if (isset($_COOKIE[$cookieName])) {
    $cookieData = $_COOKIE[$cookieName];
    echo "Cookie value: " . $cookieData;
} else {
    echo "Cookie not set.";
}
?>