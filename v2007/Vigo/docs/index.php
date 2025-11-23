<?php
  define('INCDIR','./');
  require INCDIR.'marktx.php';

  define('BASEDIR',dirname(__FILE__).'/');
  define('SCRIPT',basename(__FILE__));

  $page = $_GET['page'];
  $set  = $_GET['set'];

  $page = str_replace('..','',$page);
  $page = str_replace('%2F','/',$page);

  define('PAGE',$page);
  define('SET', intval($set));

  //>>>include 'mail.php';
  //>>>include '../statcounter.php';

  echo makePage();
?>