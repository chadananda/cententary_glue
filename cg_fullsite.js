
Drupal.behaviors.centenaryDailyTicker = function(context) {
  $("#daily-ticker").cycle({
    fx: 'scrollUp',
    easeIn: 'linear',
    easeOut: 'linear',
    speed: 2000,
    timeout: 6800,
    delay: -1000
  })
}
