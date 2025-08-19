import React from 'react';
import axios from 'axios';
import { Button, Modal, Form } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../../contexts';

export function AddVideoModal ({ activeIndex, setSlides, presId }) {
  const { storeData } = React.useContext(StoreContext);
  const { config } = React.useContext(AuthContext);

  const [URL, setURL] = React.useState('');
  const [width, setWidth] = React.useState(0);
  const [height, setHeight] = React.useState(0);
  const [autoPlay, setAutoPlay] = React.useState(false);

  const [show, setShow] = React.useState(false);

  const handleClose = () => setShow(false);
  const handleShow = () => setShow(true);

  const handleSwitchChange = (event) => {
    setAutoPlay(event.target.checked);
  };

  const addText = async () => {
    if (URL.length === 0) {
      alert('Please enter URL');
    }
    if (isNaN(width) || isNaN(height)) {
      alert('Please enter a number for width or height')
    }
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const userPres = userStore.store.presentations;
      const presIndex = userPres.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      const currPres = userPres[presIndex];
      const currSlide = currPres.slides[activeIndex];

      const newVideo = {
        width: width,
        height: height,
        URL: URL,
        autoPlay: autoPlay,
        zIndex: currSlide.zIndex
      };
      currSlide.zIndex++;
      currSlide.video.push(newVideo);
      storeData(userStore);
      setSlides(currPres.slides);
      handleClose();
    } catch (err) {
      alert(err.response);
    }
  }

  const enterKeyDown = (event) => {
    if (event.key === 'Enter') {
      addText();
    }
  }

  return (
    <>
    <Button variant="info" onClick={handleShow}>Add Video</Button>
    <Modal
        show={show}
        onHide={handleClose}
        backdrop="static"
        keyboard={false}
      >
        <Modal.Header closeButton>
          <Modal.Title>VIDEO</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <Form>
          <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Size</Form.Label>
              <Form.Control
                type="width"
                placeholder="width %"
                onChange={e => setWidth(e.target.value)}
                autoFocus
              />
              <Form.Control
                type="height"
                placeholder="height %"
                onChange={e => setHeight(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>URL</Form.Label>
              <Form.Control
                type="text"
                placeholder="Input URL"
                onChange={e => setURL(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Check
              type="switch"
              id="custom-switch"
              label="Auto-play"
              checked={autoPlay}
              onChange={handleSwitchChange}
            />
          </Form>
        </Modal.Body>
        <Modal.Footer>
          <Button variant="danger" onClick={handleClose}>
            Cancel
          </Button>
          <Button variant="primary" onKeyDown={enterKeyDown} onClick={addText}>Create</Button>
        </Modal.Footer>
      </Modal>
    </>
  );
}
