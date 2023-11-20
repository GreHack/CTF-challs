import { v1 as uuidv1} from 'uuid';

const { Client } = require('pg');
class Database {
  client;

  private static _instance: Database;
  private constructor() {
    this.client = new Client();
    this.initDb();
  }

  static instance() {
    if (this._instance == null) {
      this._instance = new Database();
    }

    return this._instance;
  }

  async initDb() {
    await this.client.connect();

    await this.client.query(`DROP TABLE IF EXISTS messages`);
    await this.client.query(`CREATE TABLE messages (
                                id SERIAL PRIMARY KEY,
                                uuid UUID,
                                content TEXT,
                                created TIMESTAMP
                             )`);

    const message = "Congratz! You're so strong and deserve a reward: GH{Using_GU1D_v1_is_not_th3_best!}";
    const msgUuid = uuidv1();
    console.log(`Creating admin message with uuid ${msgUuid}`);
    await this.client.query('INSERT INTO messages (uuid, content, created) VALUES ($1, $2, $3)', [msgUuid, message, (new Date()).toISOString()]);
  }
}

export default Database.instance();
