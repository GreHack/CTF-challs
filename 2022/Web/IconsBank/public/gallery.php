<?php
$images = [
  'uploads/3c895ecb69a540c6fe623f2a33ebb00fe988c05f1c631575bd7125fd5f58441f.png',
  'uploads/6bd1a6dfb16f718a03664a1debd15e8fca52ec3462b18240b82aa62b9439eee9.png',
  'uploads/6c02f8303a360b5cdcb1c62a99c8d9e17fc61e309b73c0992b7fe98629566487.png',
  'uploads/9fe155efc1e992606f94d7d97fb3e48f823bf177b933e2980811174698b4bfee.png',
  'uploads/94dee7cdee977a329f20d91cc5048d12b994cdc63658fb60620f708d1f243a39.png',
  'uploads/273c0d9e0565262d6f71e1e802dbade1902c153be2d233a4ecd7c6d1b11f9d9a.png',
  'uploads/408d30a621ecf1641992329a91879a72eace3a0a5d6b44f1b5b12332898f5c9e.png',
  'uploads/4941e4b45afc8ae3b33799864d9d359c335a1299c2aca73711c89677de74f939.png',
  'uploads/13956b1611baeaa7f01ae69a7ee81d729c15308786ee4b8ebb9abea829fdadba.png',
  'uploads/33083bb002bc116dbbc4cbd2c270688ee8325ef5dd50232fa4226cf43a261c55.png',
  'uploads/61533d03d5cbf94fccc3c604d1b5fea5323da88d4127968fd6da64c30dc8b5dc.png',
  'uploads/a079f6341aa11db71f05bdf4c188c63b12ce52909101e4d20d2ff5959c3c48fc.png',
  'uploads/a1348d4d18bb256c36c76b1c9872a611b2ea07b807aaaf109c1a60b9c4c0c690.png',
  'uploads/ab0a4d30d115a98c3828bbff9d5d4bef10bd7547a199a143f29680bb5d0dd9f4.png',
  'uploads/dddbcc51c0497e923e10e21dac7e07192a8249370026c28128326533ce2a61f4.png',
  'uploads/f148d6f611333e3fdb5d0f9da9a559966007c408bf08bdacc921e4a96f1f096b.png'
];

shuffle($images);
?>

<h1 class="text-4xl font-bold uppercase tracking-wider text-center mt-10 mb-20">Available icons</h1>

<section class="max-w-[1000px] flex flex-wrap m-auto justify-center gap-20">
  <?php
  foreach ($images as $image) {
  ?>
    <img src="<?php echo $image; ?>" alt="Icon">
  <?php
  }
  ?>
</section>
