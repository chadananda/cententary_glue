<?php
// $Id$

/**
 * @file
 *
 */

function cg_settings_form() {
  $form['facebook'] = array(
    '#type' => 'fieldset',
    '#title' => t('Facebook'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['facebook']['cg_fb_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#default_value' => variable_get('cg_fb_app_id', ''),
  );
  $form['facebook']['cg_fb_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App Secret'),
    '#default_value' => variable_get('cg_fb_app_secret', ''),
  );
//  $form['facebook']['test_facebook'] = array(
//    '#type' => 'submit',
//    '#value' => 'Test Facebook',
//  );

  $form['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['twitter']['cg_twitter_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Key'),
    '#default_value' => variable_get('cg_twitter_key', ''),
  );
  $form['twitter']['cg_twitter_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Secret'),
    '#default_value' => variable_get('cg_twitter_secret', ''),
  );
//  $form['twitter']['test_twitter'] = array(
//    '#type' => 'submit',
//    '#value' => 'Test Twitter',
//  );

  $form['#submit'] = array('cg_settings_form_submit'); 

  return system_settings_form($form);
}

function cg_settings_form_submit(&$form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == 'Test Facebook') {
  }
  elseif ($form_state['clicked_button']['#value'] == 'Test Twitter') {
  }
}

function cg_daily_archive($date = FALSE, $all = FALSE) {

  // CONTENT
  if (!$date = cg_daily_archive_date($date)) {
    drupal_goto('daily-archive/1912-' . format_date(time(), 'custom', 'm'));
  }
  drupal_set_html_head("<link rel='canonical' href='http://centenary.bahai.us/daily-archive/$date' />");
  if (($cache = cache_get('cg:daily-archive-' . $date, 'cache_page'))
      && !empty($cache->data)
      && (time() < $cache->expire))
  {
    $output = $cache->data;
  }
  else {
    $rows = cg_daily_archive_data(cg_daily_archive_date($date));
    drupal_set_title("Daily Centenarian Archive : " . format_date(strtotime($date), 'custom', 'F Y'));
    if (!is_array($rows)) {
      $output = t("<p>There are no pages available for this month.</p>");
    }
    else {
      foreach ($rows as $r) {
        $display_date = format_date(strtotime($r->date), 'custom', 'D, F j, Y');
        $title = check_plain(cg_fix_bahai_words($r->title));
        $img = '';
        $link = "/daily/$r->date";
        $more = format_plural($r->count, '1 article', '@count articles');
        $image_markup = '';
        if ($r->img) {
          $img = imagecache_create_url('teaser', $r->img);
          $image_markup = "<div class='views-field-field-main-image-fid'>
            <a href='$link'><img width='55' height='55' src='$img' class='imagecache imagecache-teaser imagecache-default imagecache-teaser_default'></a>
          </div>";
        }
        $text = <<<EOF
<div class="view-id-daily view-display-id-block_1">
  <div class="view-header">
    <p><a href="$link" class="daily-centenarian-link"><span class="daily-headline">$display_date</span></a></p>
  </div>
  <div class="view-content">
    $image_markup
    <div class="views-field-title">
      <a href="$link">$title</a>
    </div>
    <div class="views-field-nothing">
      <div class="more-link"><a href="$link">$more</a></div>
    </div>
  </div>
</div>
EOF;
        $output = $text . $output;
      }
    }

    // NAVIGATION
    $thisdate = format_date(time(), 'custom', 'y-m-d');
    if ($lastdate = db_result(db_query("SELECT d.field_historical_date_value
      FROM {node} n LEFT JOIN {content_field_historical_date} d USING (vid) WHERE
      n.status AND n.type IN ('clip', 'talk', 'vignette') AND
      d.field_historical_date_value < '%s' AND d.field_historical_date_value >= '1911-01-01T00:00:00'
      ORDER BY d.field_historical_date_value DESC LIMIT 0,1", "$date-01"))) {
      $links = '<div class="daily-archive-link prev">' .
        l('<< ' . format_date(strtotime($lastdate), 'custom', 'F Y'),
          'daily-archive/' . substr($lastdate, 0, 7))
          . '</div>';
    }
    if ($nextdate = db_result(db_query("SELECT d.field_historical_date_value
      FROM {node} n LEFT JOIN {content_field_historical_date} d USING (vid) WHERE
      n.status AND n.type IN ('clip', 'talk', 'vignette') AND
      d.field_historical_date_value >= '%s' AND d.field_historical_date_value <= '1913-12-31T00:00:00' 
      ORDER BY d.field_historical_date_value ASC LIMIT 0,1", format_date(strtotime("$date +1 month"), 'custom', 'Y-m')))) {
      $links .= '<div class="daily-archive-link next">' .
        l(format_date(strtotime($nextdate), 'custom', 'F Y') . ' >>',
          'daily-archive/' . substr($nextdate, 0, 7))
          . '</div>';
    }
    $links = "<div class='daily-archive-nav-links'>$links</div>";
    $nav .= '<div class="daily-archive-month-links clearfix">';
    $current_month = format_date(time(), 'custom', 'y-m');
    for ($t = 1; $t <= 12; $t++) {
      $m = str_pad($t, 2, '0', STR_PAD_LEFT);
      $nav .= l(format_date(strtotime("1912-$m"), 'custom', 'M'), "daily-archive/1912-$m");
    }
    $nav .= '</div>';
    $output = $links . $nav . $output;
    $expire = strtotime(format_date(time(), 'custom', 'Y-m-d 06:00:00') . ' +1 day');
    cache_set('cg:daily-archive-' . $date, $output, 'cache_page', $expire);
  }
  return $output;
}

function cg_daily_archive_rss() {
  if (($cache = cache_get('cg:daily-archive-rss', 'cache'))
      && !empty($cache->data)
      && (time() < $cache->expire))
  {
    $output = $cache->data;
  }
  else {
    global $base_url, $language;
    $rows = cg_daily_archive_data('1912', FALSE);
    $items = '';
    foreach ($rows as $r) {

      // Link
      $display_date = format_date(strtotime($r->date), 'custom', 'D, M j, Y');
      $link = url("daily/$r->date", array('absolute' => TRUE));

      // Title
      $title = '"' . $r->title . '"' .
        (($n = $r->count - 1) ? format_plural($n, ' and 1 other article', ' and @count other articles') : '');

      // Extras
      $extras = array();
      $e = array(
        'pubDate' => gmdate('r', strtotime("$r->date +100 years")),
        'dc:creator' => 'Centenary Team',
        'guid' => "http://centenary.bahai.us/daily/$r->date",
      );
      foreach ($e as $k=>$v) {
        $extras[] = array('key' => $k, 'value' => $v);
      }

      if ($r->img) {
        // Image
        $extras[] = array(
          'key' => 'enclosure',
          'attributes' => array(
            'url' => imagecache_create_url('page-main-image', $r->img),
            'type' => $r->filemime,
            'length' => $r->filesize,
          ),
        );
      }
      $item = format_rss_item("The Daily Centenarian for $display_date", $link, $title, $extras);
      if ($r->img) {
        $item = str_replace('<description>', '<description><![CDATA[<a href="' .
          $base_url . '/daily/' . $r->date . '"> <img align="left" hspace="5" src="' .
          imagecache_create_url('teaser', $r->img) . '"/></a> ', $item);
        $item = str_replace('</description>', "]]>\n</description>", $item);
      }
      $items .= $item;
    }
    $channel = array(
      'title' => 'The Daily Centenarian',
      'link' => 'http://centenary.bahai.us/daily',
      'description' => "The internet source for news, talks, stories and personal
   recollections surrounding 'Abdu'l-Bahá's journeys through America in 1912.",
    );
    $extras = array(
      array(
        'key' => 'image',
        'attributes' => array(
          'link' => $base_url . '/daily',
          'title' => 'The Daily Centenarian',
          'url' => $base_url . '/sites/all/themes/centenary/images/daily-icon.png',
          'height' => '149',
          'width' => '210',
        ),
      ),
    );
    $namespaces = array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/');

    $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    $output .= "<rss version=\"2.0\" xml:base=\"http://centenary.bahai.us/daily\" " . drupal_attributes($namespaces) . ">\n";
    $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], $items, $language->language, $extras);
    $output .= "</rss>\n";
    $expire = strtotime(format_date(time(), 'custom', 'Y-m-d 06:00:00') . ' +1 day');
    cache_set('cg:daily-archive-rss', $output, 'cache', $expire);
  }

  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');
  print $output;
}

function cg_daily_archive_date($date = FALSE) {
  $matches = array();
  if (!preg_match('|\d\d\d\d-\d\d|', $date, $matches) || !$date = $matches[0]) {
    return FALSE;
  }
  return $date;
}

function cg_daily_archive_data($month, $all = TRUE) {

  if (!preg_match('|^191[1-3]|', $month)) {
    return FALSE;
  }

  // Text for the final query
  $query = cg_daily_query();

  // build the date query
  $where = ("n.status AND n.type IN ('clip', 'talk', 'vignette') AND
    d.field_historical_date_value BETWEEN '%s' AND '%s'");
  $args = array("$month-01", "$month-31");
  // The code below was needed when we hid dates la
//  if (!$all) {
//    $where .= " AND d.field_historical_date_value <= '%s'";
//    $args[] = '1912-' . format_date(time(), 'custom', 'm-d') . 'T00:00:00';
//  }
  $datequery = db_query("SELECT COUNT(n.nid) AS count,
    d.field_historical_date_value AS date FROM {node} n
    LEFT JOIN {content_field_historical_date} d USING (vid)
    WHERE $where GROUP BY d.field_historical_date_value
    ORDER BY d.field_historical_date_value DESC LIMIT 0,31", $args);

  // fetch the real data
  while($date = db_fetch_object($datequery)) {
    $data[$date->date] = db_fetch_object(db_query($query, $date->date));
    $data[$date->date]->count = $date->count;
    $data[$date->date]->date = str_replace('T00:00:00', '', $data[$date->date]->date);
  }
  return $data;
}
