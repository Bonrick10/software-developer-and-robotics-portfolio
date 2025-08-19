import React from 'react';
import { Button, Modal, Form } from 'react-bootstrap';

export function EditTitleModal ({ show, onHide, onOpen, currTitle, editTitle }) {
  const [title, setTitle] = React.useState('');

  const editModalTitle = () => {
    editTitle(title);
    onHide();
  }

  return (
    <>
      <Button variant="secondary" onClick={onOpen}>
        Edit Title
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
                defaultValue={currTitle}
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
          <Button variant="primary" onClick={editModalTitle}>Create</Button>
        </Modal.Footer>
      </Modal>
    </>
  );
}
