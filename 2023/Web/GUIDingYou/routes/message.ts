import { createMessage, getMessage, listMessages } from '../controllers/messageController';

const express = require('express');
const router = express.Router();

router.post('/', createMessage);
router.get('/', listMessages);
router.get('/:id/', getMessage);

module.exports = router;
