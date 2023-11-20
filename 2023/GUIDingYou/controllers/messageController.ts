import { Request, Response } from 'express';
import Database from '../database';
import { v1 as uuidv1, validate } from 'uuid';

export async function createMessage(req: Request, res: Response) {
  console.log(`Create message "${req.body.message}".`)
  if (req.body.message == null) {
    return res.sendStatus(400);
  }

  const result = await Database.client.query('INSERT INTO messages (uuid, content, created) VALUES ($1, $2, $3) RETURNING *', [uuidv1(), req.body.message, (new Date()).toISOString()]);
  res.json((result.rows !== null) ? result.rows : []);

}

export async function listMessages(req: Request, res: Response) {
  const result = await Database.client.query('SELECT id, created FROM messages;');
  res.json((result.rows !== null) ? result.rows : [])
}

export async function getMessage(req: Request, res: Response) {
  console.log(`Get /${req.params.id} with uuid ${req.query.uuid}`);
  const id = Number(req.params.id);
  if (Number.isNaN(id) || req.query.uuid == null || !validate(<string>req.query.uuid)) {
    return res.sendStatus(400);
  }

  const result = await Database.client.query('SELECT * FROM messages WHERE id=$1 AND uuid=$2', [id, req.query.uuid]);
  res.json((result.rows !== null) ? result.rows : []);
}
