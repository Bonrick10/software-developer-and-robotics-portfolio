import React from 'react';
import axios from 'axios';
import { Button, Modal, Form } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../../contexts';

export function AddImageModal ({ activeIndex, setSlides, presId }) {
  const { config } = React.useContext(AuthContext);
  const { storeData } = React.useContext(StoreContext);

  const [width, setWidth] = React.useState(0);
  const [height, setHeight] = React.useState(0);
  const [src, setSrc] = React.useState('');
  const [alt, setAlt] = React.useState('');

  const [modalShow, setModalShow] = React.useState(false);
  const handleClose = () => setModalShow(false);
  const handleOpen = () => setModalShow(true);

  const getBase64 = (file) => {
    const reader = new FileReader();
    reader.readAsDataURL(file);
    reader.onload = function () {
      setSrc(reader.result);
    };
    reader.onerror = (error) => {
      alert('Error: ', error);
    };
  }

  const createImage = async () => {
    if (!width || !height || !src || !alt) {
      alert('Please fill empty fields');
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

      const newImage = {
        width: width,
        height: height,
        src: src,
        alt: alt,
        zIndex: currSlide.zIndex
      }
      currSlide.zIndex++
      currSlide.image.push(newImage);
      storeData(userStore);
      setSlides(currPres.slides);
      handleClose();
    } catch (err) {
      console.log(err);
      alert(err.response);
    }
  }

  return (
    <>
      <Button variant="info" onClick={handleOpen}>
        Add Image
      </Button>
      <Modal
        show={modalShow}
        onHide={handleClose}
        backdrop="static"
        keyboard={false}
      >
        <Modal.Header closeButton>
          <Modal.Title>Add Image</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <Form>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Width</Form.Label>
              <Form.Control
                type="number"
                min="0.001"
                step=".001"
                placeholder="Width %"
                onChange={e => setWidth(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Height</Form.Label>
              <Form.Control
                type="number"
                min="0.001"
                step=".001"
                placeholder="Height %"
                onChange={e => setHeight(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Image File</Form.Label>
              <Form.Control
                type="file"
                onChange={e => getBase64(e.target.files[0])}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Alt Text</Form.Label>
              <Form.Control
                type="text"
                placeholder="Alternate text for the image"
                onChange={e => setAlt(e.target.value)}
                autoFocus
              />
            </Form.Group>
          </Form>
        </Modal.Body>
        <Modal.Footer>
          <Button variant="danger" onClick={handleClose}>
            Cancel
          </Button>
          <Button variant="primary" onClick={createImage}>Create</Button>
        </Modal.Footer>
      </Modal>
    </>
  );
}
