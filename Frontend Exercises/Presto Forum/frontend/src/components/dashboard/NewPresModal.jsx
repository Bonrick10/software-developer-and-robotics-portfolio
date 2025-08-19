import React from 'react';
import axios from 'axios';
import { Button, Modal, Form } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../contexts';

export function NewPresModal ({ show, onOpen, onHide }) {
  const { config } = React.useContext(AuthContext);
  const { storeData } = React.useContext(StoreContext);

  const [title, setTitle] = React.useState('');

  const createPres = async () => {
    if (title.length === 0) {
      alert('Please enter a title');
    }
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;

      if (!userStore.store.presentations) {
        userStore.store.presentations = [];
      }

      if (!userStore.store.presCount) {
        userStore.store.presCount = 1;
      }

      const newPres = {
        title: title,
        presId: userStore.store.presCount,
        slideCount: 1,
        saveStates: [],
        slides: [{
          zIndex: 1,
          slideId: 0,
          backgroundColour: '',
          gradientDirection: '',
          gradientColourOne: '',
          gradientColourTwo: '',
          colour: true,
          gradient: true,
          text: [],
          video: [],
          image: [],
          code: []
        }],
      };
      userStore.store.presCount++;
      const presArray = userStore.store.presentations;
      presArray.push(newPres);
      storeData(userStore);
      onHide();
    } catch (err) {
      console.log(err);
      alert(err.response);
    }
  }

  return (
    <>
      <Button variant="primary" onClick={onOpen}>
        New Presentation
      </Button>
      <Modal
        show={show}
        onHide={onHide}
        backdrop="static"
        keyboard={false}
      >
        <Modal.Header closeButton>
          <Modal.Title>New Presentation</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <Form>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Title</Form.Label>
              <Form.Control
                type="text"
                placeholder="Title of the new presentation"
                onChange={e => setTitle(e.target.value)}
                autoFocus
              />
            </Form.Group>
          </Form>
        </Modal.Body>
        <Modal.Footer>
          <Button variant="danger" onClick={onHide}>
            Cancel
          </Button>
          <Button variant="primary" onClick={createPres}>Create</Button>
        </Modal.Footer>
      </Modal>
    </>
  );
}
