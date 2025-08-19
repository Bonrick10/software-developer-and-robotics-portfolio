import React from 'react';
import axios from 'axios';
import { Link } from 'react-router-dom';
import { Card } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../contexts';

export function ListPresentation () {
  const { config } = React.useContext(AuthContext);
  const { storeData } = React.useContext(StoreContext);
  const [presentations, setPresentations] = React.useState([]);

  React.useEffect(() => {
    const getData = async () => {
      try {
        const response = await axios.get('http://localhost:5005/store', config);
        const userStore = response.data.store;
        if (userStore.store.presentations) {
          setPresentations(userStore.store.presentations);
        }
      } catch (err) {
        console.log(err);
        alert(err.response);
      }
    }
    getData();
  }, [storeData]);
  return <div className="Grid">{presentations.map((card, index) => {
    return (
      <Link key={index} to={`/editPres/${card.presId}/slide/0`} style={{ textDecoration: 'none' }}>
        <Card style={{ width: '18rem' }} className="Card">
        <Card.Img variant="top" className="Thumbnail" /* src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/18/Grey_Square.svg/1024px-Grey_Square.svg.png" *//>
          <Card.Body>
            <Card.Title>Title: {card.title}</Card.Title>
            <Card.Text>Description: </Card.Text>
            <Card.Text>Number of slides: {card.slides.length}</Card.Text>
          </Card.Body>
        </Card>
      </Link>
    )
  })}</div>
}
