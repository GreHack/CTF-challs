<?php
function generateRandomString($length = 10) {
  $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  $charactersLength = strlen($characters);
  $randomString = '';
  for ($i = 0; $i < $length; $i++) {
    $randomString .= $characters[rand(0, $charactersLength - 1)];
  }
  return $randomString;
}

$alert = null;

function upload($file) {
  global $alert;

  $tmpFilename = $file['tmp_name'];
  $target_dir = "uploads/";

  if (!is_uploaded_file($tmpFilename)) {
    $alert = 'The file has not been uploaded correctly.';
    return '';
  }

  $mimeType = mime_content_type($tmpFilename);
  if ($mimeType !== 'image/png') {
    $alert = 'Only PNG files are supported yet.';
    return '';
  }

  try {
    # Compress the uploaded PNG (level 9 of the zlib library),
    # resize it and save it
    list($width, $height) = getimagesize($tmpFilename);
    $source = imagecreatefrompng($tmpFilename);
    $thumb = imagecreatetruecolor(55, 55);
    imagecopyresampled($thumb, $source, 0, 0, 0, 0, 55, 55, $width, $height);
    imagecolortransparent($thumb, imagecolorallocate($thumb, 0, 0, 0));

    ob_start();
    imagepng($thumb);
    $imageContents = ob_get_clean();

    $target_file = $target_dir . hash('sha256', generateRandomString(20) . $imageContents) . '.png';
    imagepng($thumb, $target_file);

    return $target_file;
  } catch (FileException $e) {
    http_response_code(500);
    $alert = 'A server error occurred.';
  }

  return '';
}


if (isset($_FILES['icon'])) {
  $imagePath = upload($_FILES['icon']);
}

if ($alert !== null) {
?>
  <div class="absolute right-5 top-0 cursor-pointer" onclick="this.remove()">
    <div class="alert alert-error shadow-lg mt-5">
      <svg xmlns="http://www.w3.org/2000/svg" class="stroke-current flex-shrink-0 h-6 w-6" fill="none" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M10 14l2-2m0 0l2-2m-2 2l-2-2m2 2l2 2m7-2a9 9 0 11-18 0 9 9 0 0118 0z" /></svg>
      <span><?php echo $alert; ?></span>
    </div>
  </div>
<?php
}

if (isset($imagePath) && $imagePath !== '') {
?>
  <div class="absolute right-5 top-0 cursor-pointer" onclick="this.remove()">
    <div class="alert alert-success shadow-lg mt-5">
      <svg xmlns="http://www.w3.org/2000/svg" class="stroke-current flex-shrink-0 h-6 w-6" fill="none" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z" /></svg>
      <span>Your file has been uploaded. It is under review by an administrator.</span>
    </div>
  </div>
<?php
}
?>

<div class="flex flex-col h-full justify-center items-center gap-20">
  <form action="" method="post" enctype="multipart/form-data" class="flex flex-col gap-5">
    <h2 class="text-3xl font-bold mb-8 uppercase tracking-widest">Send a new image</h2>
    <input type="file" name="icon" class="border cursor-pointer pr-2 file:cursor-pointer input-primary text-sm rounded-lg cursor-pointer focus:outline-none file:bg-primary file:border-0 file:text-sm file:font-semibold file:uppercase file:text-primary file:p-3 file:tracking-wider">
    <button type="submit" class="btn btn-active btn-primary tracking-wider">Submit new icon</button>
  </form>

  <?php
  if (isset($imagePath) && $imagePath !== '') {
  ?>
    <img src="<?php echo $imagePath ?>" alt="uploaded image" />
  <?php
  }
  ?>
</div>
