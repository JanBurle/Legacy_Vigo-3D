<?php
  if(isset($_POST["sendmsg"]))
  {
    if(mail('<ian_b@users.sourceforge.net>',"[vigo] $page",$_POST["sendmsg"]))
    {
      echo '<script>alert("Your message has been sent.")</script>';
    }
  }
?>
